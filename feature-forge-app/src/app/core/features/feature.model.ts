export type FeatureType = 'ab-test' | 'release' | 'ops' | 'permission';

export interface FeatureFlag<T = any> {
  readonly id: string;
  readonly type: FeatureType;
  readonly enabled: boolean;
  /** * The 'payload' allows for dynamic data. 
   * A/B tests use this for variant names ('A', 'B', 'C').
   * Canary releases might use this for rollout percentages.
   */
  readonly payload?: T;
}

/**
 * Abstract provider so we can swap between Mocks and Real APIs
 * without the rest of the app noticing.
 */
export abstract class FeatureProvider {
  abstract fetchFlags(): Promise<FeatureFlag[]>;
}