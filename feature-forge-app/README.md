# Feature Forge

<img width="1182" height="385" alt="image" src="https://github.com/user-attachments/assets/9926eebb-a922-4371-9e74-3b5ff09fc0ba" />



---

A modular Angular 21 application demonstrating the **Strategy Pattern** for dynamic UI delivery and feature flagging. This project serves as an experimental sandbox for decoupled business logic and modern styling pipelines.

---

### 🛠 Architecture: The Strategy Pattern
Instead of using complex conditionals within components, this app delegates UI logic to interchangeable strategy classes. This allows for seamless transitions between different user experiences:

* **Standard Strategy**: The default, clean user experience.
* **Experimental Strategy**: A high-contrast A/B test variant (Indigo theme).
* **Maintenance Strategy**: An operational toggle for system downtime (Red theme).

---

### Tech Stack
* **Framework**: Angular 21 (using the high-performance `esbuild` application builder).
* **Styling**: 
    * **Tailwind CSS v4**: Utilizing the new CSS-native configuration and `@source` detection.
    * **Sass (SCSS)**: Preserved for component-level styling logic.
* **PostCSS**: Configured via `.postcssrc.json` to bridge the Tailwind/Sass parallel pipeline.

---

### Getting Started

**1. Install Dependencies**
```bash
npm install
```

**2. Run Development Server**

```bash
npm start
```

**3. Build for Production**

```bash
npm run build
```

---

### Key Files

* **`src/app/hero.strategy.ts`**: The interface defining the strategy contract.
* **`src/app/hero.implementations.ts`**: Concrete logic for Standard, Experimental, and Maintenance modes.
* **`src/tailwind.css`**: The Tailwind v4 entry point with custom `@source` scanning for TypeScript files.
* **`angular.json`**: Configured with a parallel style array to support both `.css` and `.scss` processing.

---

### Note on Tailwind v4 Integration

This project successfully solves the **Sass vs. Tailwind v4** conflict in Angular by utilizing a parallel build path, ensuring that Tailwind's JIT engine can scan TypeScript strings for utility classes without interference from the Sass compiler.



