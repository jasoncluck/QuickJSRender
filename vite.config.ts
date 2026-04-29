import { defineConfig } from 'vite';

export default defineConfig({
  build: {
    lib: {
      entry: './src/main.ts',
      name: 'MyBundle',
      fileName: () => 'bundle.js',
      formats: ['iife'], // Simple format for QuickJS to execute
    },
    outDir: 'dist',
    minify: false,
  },
});
