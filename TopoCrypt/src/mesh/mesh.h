#ifndef TOPO_MESH_H
#define TOPO_MESH_H
/*
 * Simple deterministic triangulated mesh core
 * C99, minimal dependencies
 *
 * Public API for initial scaffold implementation.
 */

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Error codes */
enum {
    MESH_OK = 0,
    MESH_ERR_OOM = 1,
    MESH_ERR_INVALID_ARG = 2,
    MESH_ERR_IO = 3,
    MESH_ERR_NOT_FOUND = 4,
    MESH_ERR_CAPACITY = 5
};

/* Basic types */
typedef struct mesh_t mesh_t;

/* Create/destroy */
mesh_t *mesh_create(uint32_t vertex_capacity, uint32_t face_capacity);
void mesh_destroy(mesh_t *m);

/* Simple accessors */
uint32_t mesh_vertex_count(const mesh_t *m);
uint32_t mesh_face_count(const mesh_t *m);

/* Vertex operations */
int mesh_add_vertex(mesh_t *m, uint32_t *out_vid); /* returns vertex id in out_vid */
int mesh_set_vertex_flag(mesh_t *m, uint32_t vid, uint32_t flags);
int mesh_get_vertex_flag(const mesh_t *m, uint32_t vid, uint32_t *out_flags);

/* Face operations: faces are triples of vertex indices (v0, v1, v2) */
int mesh_add_face(mesh_t *m, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t *out_fid);
int mesh_remove_face(mesh_t *m, uint32_t fid);

/* Attach a localized block mesh into m by identifying an edge (attach_v0, attach_v1)
 * block is not modified. Caller is responsible for providing a block whose local
 * vertex indices are contiguous and the attach edge exists in block.
 * On success, out_face_start receives the starting face index in m where block faces were appended.
 */
int mesh_attach_local_block(mesh_t *m, const mesh_t *block, uint32_t attach_v0, uint32_t attach_v1, uint32_t *out_face_start);

/* Permutation utilities
 * perm is an array of length vertex_count() mapping old_index -> new_index
 * mesh_apply_permutation permutes vertex labels in-place.
 * mesh_invert_permutation computes inverse permutation inv such that inv[perm[i]] == i
 */
int mesh_apply_permutation(mesh_t *m, const uint32_t *perm, size_t n);
int mesh_invert_permutation(const uint32_t *perm, size_t n, uint32_t *out_inv);

/* Serialization: canonical binary format (little-endian)
 * These functions produce/consume the canonical face-list representation used as the public artifact core.
 */
int mesh_serialize_canonical(FILE *f, const mesh_t *m);
mesh_t *mesh_deserialize_canonical(FILE *f);

/* Text export for debugging: deterministic face-list text */
int mesh_export_face_list_text(FILE *f, const mesh_t *m);

/* Lightweight consistency check (returns MESH_OK or error) */
int mesh_check_consistency(const mesh_t *m);

#ifdef __cplusplus
}
#endif

#endif /* TOPO_MESH_H */
