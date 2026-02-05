import { Injectable, signal, computed, inject } from '@angular/core';
import { FeatureFlag, FeatureProvider } from './feature.model';

@Injectable({ providedIn: 'root' })
export class FeatureService {
  private readonly provider = inject(FeatureProvider);
  
  // The primary state: a Readonly Map for O(1) lookups
  private readonly _flags = signal<Map<string, FeatureFlag>>(new Map());
  
  // Publicly exposed signal for components to track
  readonly allFlags = computed(() => Array.from(this._flags().values()));

  /**
   * Initializer called during app startup
   */
  async init(): Promise<void> {
    try {
      const flags = await this.provider.fetchFlags();
      this._flags.set(new Map(flags.map(f => [f.id, f])));
    } catch (error) {
      console.error('FeatureForge: Failed to load flags', error);
      // Fail-safe: empty map means all isEnabled() calls return false
      this._flags.set(new Map());
    }
  }

  /**
   * Simple check for booleans (Ops, Release, Permissions)
   */
  isEnabled(id: string): boolean {
    return this._flags().get(id)?.enabled ?? false;
  }

  /**
   * Get the payload for complex types (A/B Test variants)
   */
  getPayload<T>(id: string): T | undefined {
    return this._flags().get(id)?.payload as T;
  }
}