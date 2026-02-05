import { Injectable } from '@angular/core';
import { HeroStrategy } from './hero.strategy';

/**
 * STANDARD STRATEGY
 * The default experience for most users.
 */
@Injectable({ providedIn: 'root' })
export class StandardHeroStrategy implements HeroStrategy {
  getTitle() { return 'Build the Future'; }
  getDescription() { return 'The ultimate toolkit for Angular 21 developers.'; }
  getCtaLabel() { return 'Get Started'; }
  getContainerClasses() { return 'bg-white text-slate-900 border-slate-200'; }
  getButtonClasses() { return 'bg-slate-900 text-white hover:bg-slate-800'; }
  getVisualComponent() { return null; }
}

/**
 * EXPERIMENTAL STRATEGY (A/B Test Variant)
 * Higher contrast, different copy to drive engagement.
 */
@Injectable({ providedIn: 'root' })
export class ExperimentalHeroStrategy implements HeroStrategy {
  getTitle() { return 'Experience the Forge'; }
  getDescription() { return 'Next-gen feature flagging at your fingertips.'; }
  getCtaLabel() { return 'Join the Beta'; }
  getContainerClasses() { return 'bg-indigo-600 text-white border-transparent shadow-xl'; }
  getButtonClasses() { return 'bg-white text-indigo-600 hover:bg-indigo-50'; }
  getVisualComponent() { return null; }
}

/**
 * MAINTENANCE STRATEGY (Ops Toggle)
 * Used for emergency shut-offs or scheduled downtime.
 */
@Injectable({ providedIn: 'root' })
export class MaintenanceHeroStrategy implements HeroStrategy {
  getTitle() { return 'System Tuning'; }
  getDescription() { return 'We are currently optimizing our systems. We will be back shortly.'; }
  getCtaLabel() { return 'Check Status'; }
  getContainerClasses() { return 'bg-red-50 text-red-900 border-red-200'; }
  getButtonClasses() { return 'bg-red-600 text-white hover:bg-red-700'; }
  getVisualComponent() { return null; }
}