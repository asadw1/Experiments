import { Injectable } from '@angular/core';
import { FeatureFlag, FeatureProvider } from './feature.model';

@Injectable()
export class MockFeatureProvider implements FeatureProvider {
  async fetchFlags(): Promise<FeatureFlag[]> {
    return [
      { 
        id: 'maintenance-mode', 
        type: 'ops', 
        enabled: false 
      },
      { 
        id: 'hero-ab-test', 
        type: 'ab-test', 
        enabled: true, 
        payload: { variant: 'B', color: '#6366f1' } 
      },
      { 
        id: 'new-search-layout', 
        type: 'release', 
        enabled: true 
      },
      { 
        id: 'premium-dashboard', 
        type: 'permission', 
        enabled: false 
      }
    ];
  }
}