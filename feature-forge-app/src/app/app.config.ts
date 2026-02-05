import { 
  ApplicationConfig, 
  provideBrowserGlobalErrorListeners, 
  provideAppInitializer, 
  inject 
} from '@angular/core';
import { provideRouter } from '@angular/router';

import { routes } from './app.routes';
import { FeatureService } from './core/features/feature.service';
import { FeatureProvider } from './core/features/feature.model';
import { MockFeatureProvider } from './core/features/mock-feature.provider';

// Import our strategies
import { HeroStrategy } from './features/hero/hero.strategy';
import { 
  StandardHeroStrategy, 
  ExperimentalHeroStrategy, 
  MaintenanceHeroStrategy 
} from './features/hero/hero.implementations';

export const appConfig: ApplicationConfig = {
  providers: [
    provideBrowserGlobalErrorListeners(),
    provideRouter(routes),
    
    { provide: FeatureProvider, useClass: MockFeatureProvider },

    provideAppInitializer(() => {
      const featureService = inject(FeatureService);
      return featureService.init();
    }),

    // --- The Strategy Dispatcher ---
    {
      provide: HeroStrategy,
      useFactory: () => {
        const fs = inject(FeatureService);
        
        // Priority 1: Ops / Kill-Switch
        if (fs.isEnabled('maintenance-mode')) {
          return inject(MaintenanceHeroStrategy);
        }

        // Priority 2: A/B Testing / Canary
        const abTest = fs.getPayload<{ variant: string }>('hero-ab-test');
        if (abTest?.variant === 'B') {
          return inject(ExperimentalHeroStrategy);
        }

        // Priority 3: Fallback / Default
        return inject(StandardHeroStrategy);
      }
    }
  ]
};