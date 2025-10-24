/*
 * Minimal deterministic mesh core implementation
 *
 * Implements a simple contiguous storage mesh with:
 *  - vertex array (currently only flags)
 *  - face array (triples of uint32 vertex indices)
 *
 * Not a full half-edge implementation yet; intended as a stable scaffold.
 */

#include "mesh.h"
#include <stdlib.h>
#include <string.h>

/* Internal representation */
struct mesh_t {
    uint32_t vertex_capacity;
    uint32_t face_capacity;

    uint32_t vertex_count;
    uint32_t face_count;

    /* Vertex flags or small metadata */
    uint32_t *vertex_flags; /* length = vertex_capacity */

    /* Faces stored as contiguous triples: faces[fid*3 + 0..2] */
    uint32_t *faces;        /* length = face_capacity * 3 */
};

/* Helpers */
static int ensure_vertex_capacity(mesh_t *m, uint32_t min_cap) {
    if (m->vertex_capacity >= min_cap) return MESH_OK;
    uint32_t newcap = m->vertex_capacity ? m->vertex_capacity * 2 : 16;
    while (newcap < min_cap) newcap *= 2;
    uint32_t *nv = (uint32_t*)realloc(m->vertex_flags, sizeof(uint32_t) * newcap);
    if (!nv) return MESH_ERR_OOM;
    /* zero new space */
    memset(nv + m->vertex_capacity, 0, sizeof(uint32_t) * (newcap - m->vertex_capacity));
    m->vertex_flags = nv;
    m->vertex_capacity = newcap;
    return MESH_OK;
}

static int ensure_face_capacity(mesh_t *m, uint32_t min_cap) {
    if (m->face_capacity >= min_cap) return MESH_OK;
    uint32_t newcap = m->face_capacity ? m->face_capacity * 2 : 16;
    while (newcap < min_cap) newcap *= 2;
    uint32_t *nf = (uint32_t*)realloc(m->faces, sizeof(uint32_t) * newcap * 3);
    if (!nf) return MESH_ERR_OOM;
    /* zero new space */
    memset(nf + m->face_capacity*3, 0, sizeof(uint32_t) * (newcap - m->face_capacity) * 3);
    m->faces = nf;
    m->face_capacity = newcap;
    return MESH_OK;
}

/* Public API */

mesh_t *mesh_create(uint32_t vertex_capacity, uint32_t face_capacity) {
    mesh_t *m = (mesh_t*)calloc(1, sizeof(mesh_t));
    if (!m) return NULL;
    m->vertex_capacity = 0;
    m->face_capacity = 0;
    m->vertex_count = 0;
    m->face_count = 0;
    m->vertex_flags = NULL;
    m->faces = NULL;
    if (vertex_capacity) {
        if (ensure_vertex_capacity(m, vertex_capacity) != MESH_OK) { mesh_destroy(m); return NULL; }
    }
    if (face_capacity) {
        if (ensure_face_capacity(m, face_capacity) != MESH_OK) { mesh_destroy(m); return NULL; }
    }
    return m;
}

void mesh_destroy(mesh_t *m) {
    if (!m) return;
    free(m->vertex_flags);
    free(m->faces);
    free(m);
}

uint32_t mesh_vertex_count(const mesh_t *m) {
    return m ? m->vertex_count : 0;
}

uint32_t mesh_face_count(const mesh_t *m) {
    return m ? m->face_count : 0;
}

int mesh_add_vertex(mesh_t *m, uint32_t *out_vid) {
    if (!m) return MESH_ERR_INVALID_ARG;
    uint32_t need = m->vertex_count + 1;
    int rc = ensure_vertex_capacity(m, need);
    if (rc != MESH_OK) return rc;
    uint32_t vid = m->vertex_count++;
    m->vertex_flags[vid] = 0;
    if (out_vid) *out_vid = vid;
    return MESH_OK;
}

int mesh_set_vertex_flag(mesh_t *m, uint32_t vid, uint32_t flags) {
    if (!m) return MESH_ERR_INVALID_ARG;
    if (vid >= m->vertex_count) return MESH_ERR_NOT_FOUND;
    m->vertex_flags[vid] = flags;
    return MESH_OK;
}

int mesh_get_vertex_flag(const mesh_t *m, uint32_t vid, uint32_t *out_flags) {
    if (!m || !out_flags) return MESH_ERR_INVALID_ARG;
    if (vid >= m->vertex_count) return MESH_ERR_NOT_FOUND;
    *out_flags = m->vertex_flags[vid];
    return MESH_OK;
}

int mesh_add_face(mesh_t *m, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t *out_fid) {
    if (!m) return MESH_ERR_INVALID_ARG;
    if (v0 >= m->vertex_count || v1 >= m->vertex_count || v2 >= m->vertex_count) return MESH_ERR_INVALID_ARG;
    uint32_t need = m->face_count + 1;
    int rc = ensure_face_capacity(m, need);
    if (rc != MESH_OK) return rc;
    uint32_t fid = m->face_count++;
    uint32_t idx = fid * 3;
    m->faces[idx + 0] = v0;
    m->faces[idx + 1] = v1;
    m->faces[idx + 2] = v2;
    if (out_fid) *out_fid = fid;
    return MESH_OK;
}

int mesh_remove_face(mesh_t *m, uint32_t fid) {
    if (!m) return MESH_ERR_INVALID_ARG;
    if (fid >= m->face_count) return MESH_ERR_NOT_FOUND;
    /* Simple remove by swapping last face into position to keep array compact deterministically */
    uint32_t last = m->face_count - 1;
    if (fid != last) {
        uint32_t dst = fid * 3;
        uint32_t src = last * 3;
        m->faces[dst+0] = m->faces[src+0];
        m->faces[dst+1] = m->faces[src+1];
        m->faces[dst+2] = m->faces[src+2];
    }
    m->face_count--;
    return MESH_OK;
}

int mesh_attach_local_block(mesh_t *m, const mesh_t *block, uint32_t attach_v0, uint32_t attach_v1, uint32_t *out_face_start) {
    if (!m || !block) return MESH_ERR_INVALID_ARG;
    /* Strategy: append block vertices and faces, then remap faces.
       This simple implementation does not attempt to identify duplicate vertices
       or glue along an existing edge; user must ensure attachment logic consistency.
    */
    uint32_t vcount_before = m->vertex_count;
    /* allocate space */
    for (uint32_t i = 0; i < block->vertex_count; ++i) {
        uint32_t tmp;
        int rc = mesh_add_vertex(m, &tmp);
        if (rc != MESH_OK) return rc;
    }
    /* copy vertex flags */
    for (uint32_t i = 0; i < block->vertex_count; ++i) {
        m->vertex_flags[vcount_before + i] = block->vertex_flags[i];
    }
    /* append faces with remapped indices */
    uint32_t face_start = m->face_count;
    for (uint32_t f = 0; f < block->face_count; ++f) {
        uint32_t idx = f * 3;
        uint32_t a = block->faces[idx + 0] + vcount_before;
        uint32_t b = block->faces[idx + 1] + vcount_before;
        uint32_t c = block->faces[idx + 2] + vcount_before;
        uint32_t outfid;
        int rc = mesh_add_face(m, a, b, c, &outfid);
        if (rc != MESH_OK) return rc;
    }
    if (out_face_start) *out_face_start = face_start;
    (void)attach_v0; (void)attach_v1; /* attachment glue not implemented in scaffold */
    return MESH_OK;
}

int mesh_apply_permutation(mesh_t *m, const uint32_t *perm, size_t n) {
    if (!m || !perm) return MESH_ERR_INVALID_ARG;
    if (n != m->vertex_count) return MESH_ERR_INVALID_ARG;
    /* perm maps old_index -> new_index; we must rewrite faces so that vertex indices become perm[old] */
    for (uint32_t f = 0; f < m->face_count; ++f) {
        uint32_t idx = f * 3;
        uint32_t a = m->faces[idx + 0];
        uint32_t b = m->faces[idx + 1];
        uint32_t c = m->faces[idx + 2];
        if (a >= n || b >= n || c >= n) return MESH_ERR_INVALID_ARG;
        m->faces[idx + 0] = perm[a];
        m->faces[idx + 1] = perm[b];
        m->faces[idx + 2] = perm[c];
    }
    /* Vertex flags must be permuted accordingly: produce a new flags array */
    uint32_t *new_flags = (uint32_t*)calloc(m->vertex_capacity, sizeof(uint32_t));
    if (!new_flags) return MESH_ERR_OOM;
    for (uint32_t i = 0; i < m->vertex_count; ++i) {
        uint32_t newi = perm[i];
        if (newi >= m->vertex_capacity) { free(new_flags); return MESH_ERR_INVALID_ARG; }
        new_flags[newi] = m->vertex_flags[i];
    }
    free(m->vertex_flags);
    m->vertex_flags = new_flags;
    return MESH_OK;
}

int mesh_invert_permutation(const uint32_t *perm, size_t n, uint32_t *out_inv) {
    if (!perm || !out_inv) return MESH_ERR_INVALID_ARG;
    for (size_t i = 0; i < n; ++i) out_inv[i] = UINT32_MAX;
    for (size_t i = 0; i < n; ++i) {
        uint32_t p = perm[i];
        if (p >= n) return MESH_ERR_INVALID_ARG;
        out_inv[p] = (uint32_t)i;
    }
    /* check validity */
    for (size_t i = 0; i < n; ++i) if (out_inv[i] == UINT32_MAX) return MESH_ERR_INVALID_ARG;
    return MESH_OK;
}

/* Canonical binary serialization:
 * Format:
 *  [magic 8 bytes] "TOPOC001"
 *  [u32 vertex_count]
 *  [u32 face_count]
 *  [vertex_flags array length = vertex_count u32]
 *  [faces array length = face_count * 3 u32]
 *
 * All values little-endian on-disk.
 */
static int write_u32_le(FILE *f, uint32_t v) {
    unsigned char b[4];
    b[0] = (unsigned char)(v & 0xff);
    b[1] = (unsigned char)((v >> 8) & 0xff);
    b[2] = (unsigned char)((v >> 16) & 0xff);
    b[3] = (unsigned char)((v >> 24) & 0xff);
    if (fwrite(b, 1, 4, f) != 4) return MESH_ERR_IO;
    return MESH_OK;
}

static int read_u32_le(FILE *f, uint32_t *out) {
    unsigned char b[4];
    if (fread(b, 1, 4, f) != 4) return MESH_ERR_IO;
    *out = ((uint32_t)b[0]) | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
    return MESH_OK;
}

int mesh_serialize_canonical(FILE *f, const mesh_t *m) {
    if (!f || !m) return MESH_ERR_INVALID_ARG;
    const char magic[8] = "TOPOC001";
    if (fwrite(magic, 1, 8, f) != 8) return MESH_ERR_IO;
    if (write_u32_le(f, m->vertex_count) != MESH_OK) return MESH_ERR_IO;
    if (write_u32_le(f, m->face_count) != MESH_OK) return MESH_ERR_IO;
    /* vertex flags */
    for (uint32_t i = 0; i < m->vertex_count; ++i) {
        if (write_u32_le(f, m->vertex_flags[i]) != MESH_OK) return MESH_ERR_IO;
    }
    /* faces */
    for (uint32_t i = 0; i < m->face_count * 3; ++i) {
        if (write_u32_le(f, m->faces[i]) != MESH_OK) return MESH_ERR_IO;
    }
    return MESH_OK;
}

mesh_t *mesh_deserialize_canonical(FILE *f) {
    if (!f) return NULL;
    char magic[8];
    if (fread(magic, 1, 8, f) != 8) return NULL;
    if (memcmp(magic, "TOPOC001", 8) != 0) return NULL;
    uint32_t vcount = 0, fcount = 0;
    if (read_u32_le(f, &vcount) != MESH_OK) return NULL;
    if (read_u32_le(f, &fcount) != MESH_OK) return NULL;
    mesh_t *m = mesh_create(vcount, fcount);
    if (!m) return NULL;
    m->vertex_count = vcount;
    m->face_count = fcount;
    for (uint32_t i = 0; i < vcount; ++i) {
        uint32_t vf = 0;
        if (read_u32_le(f, &vf) != MESH_OK) { mesh_destroy(m); return NULL; }
        m->vertex_flags[i] = vf;
    }
    for (uint32_t i = 0; i < fcount * 3; ++i) {
        uint32_t fv = 0;
        if (read_u32_le(f, &fv) != MESH_OK) { mesh_destroy(m); return NULL; }
        m->faces[i] = fv;
    }
    return m;
}

int mesh_export_face_list_text(FILE *f, const mesh_t *m) {
    if (!f || !m) return MESH_ERR_INVALID_ARG;
    /* deterministic: list vertices then faces */
    if (fprintf(f, "# vertices: %u\n", m->vertex_count) < 0) return MESH_ERR_IO;
    for (uint32_t i = 0; i < m->vertex_count; ++i) {
        if (fprintf(f, "v %u flags %u\n", i, m->vertex_flags[i]) < 0) return MESH_ERR_IO;
    }
    if (fprintf(f, "# faces: %u\n", m->face_count) < 0) return MESH_ERR_IO;
    for (uint32_t fi = 0; fi < m->face_count; ++fi) {
        uint32_t idx = fi * 3;
        uint32_t a = m->faces[idx+0], b = m->faces[idx+1], c = m->faces[idx+2];
        if (fprintf(f, "f %u %u %u\n", a, b, c) < 0) return MESH_ERR_IO;
    }
    return MESH_OK;
}

int mesh_check_consistency(const mesh_t *m) {
    if (!m) return MESH_ERR_INVALID_ARG;
    if (m->vertex_count > m->vertex_capacity) return MESH_ERR_INVALID_ARG;
    if (m->face_count > m->face_capacity) return MESH_ERR_INVALID_ARG;
    for (uint32_t i = 0; i < m->face_count; ++i) {
        uint32_t idx = i * 3;
        uint32_t a = m->faces[idx+0], b = m->faces[idx+1], c = m->faces[idx+2];
        if (a >= m->vertex_count || b >= m->vertex_count || c >= m->vertex_count) return MESH_ERR_INVALID_ARG;
        if (a == b || b == c || c == a) return MESH_ERR_INVALID_ARG;
    }
    return MESH_OK;
}
