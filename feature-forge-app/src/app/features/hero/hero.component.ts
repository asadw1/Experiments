import { Component, inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HeroStrategy } from './hero.strategy';

@Component({
  selector: 'app-hero',
  standalone: true,
  imports: [CommonModule],
  template: `
    <div [class]="'p-12 mb-8 rounded-3xl border transition-all duration-500 ' + strategy.getContainerClasses()">
      
      <h1 class="text-5xl font-black tracking-tight mb-4">
        {{ strategy.getTitle() }}
      </h1>
      
      <p class="text-xl opacity-90 max-w-2xl mb-8">
        {{ strategy.getDescription() }}
      </p>
      
      <button [class]="'px-8 py-4 rounded-full font-bold transition-transform active:scale-95 ' + strategy.getButtonClasses()">
        {{ strategy.getCtaLabel() }}
      </button>

      @if (strategy.getVisualComponent(); as visual) {
        <div class="mt-8">
          <ng-container *ngComponentOutlet="visual" />
        </div>
      }
    </div>
  `
})
export class HeroComponent {
  // The component simply asks for the strategy. 
  // The DI container (app.config.ts) decides WHICH one it gets.
  protected readonly strategy = inject(HeroStrategy);
}