import { Type } from '@angular/core';

/**
 * The Strategy Interface defines 'what' the UI needs.
 * It does not care 'why' it receives a specific implementation.
 */
export abstract class HeroStrategy {
  // Content methods
  abstract getTitle(): string;
  abstract getDescription(): string;
  abstract getCtaLabel(): string;
  
  // Styling methods (Tailwind classes)
  abstract getContainerClasses(): string;
  abstract getButtonClasses(): string;

  // Optional: Dynamic component injection
  // This allows the strategy to swap out the entire visual widget
  abstract getVisualComponent(): Type<any> | null;
}