using System;
using System.Collections.Generic;

namespace BlurMageddon
{
    /// <summary>
    /// NoiseInjector class provides methods for injecting noise into datasets,
    /// specifically Gaussian noise to obscure the values of numerical features.
    /// </summary>
    public class NoiseInjector
    {
        /// <summary>
        /// Injects Gaussian noise into a dataset of DataRow objects.
        /// </summary>
        /// <param name="data">A list of DataRow objects representing the dataset.</param>
        /// <param name="noiseLevel">The scale of the noise to be added (higher values result in more disruption).</param>
        public static void InjectGaussianNoise(List<DataRow> data, float noiseLevel)
        {
            // Random number generator for creating noise
            Random random = new Random();

            // Iterate through each data point in the dataset
            foreach (var row in data)
            {
                // Inject Gaussian noise into numerical features
                row.Feature1 += (float)(noiseLevel * random.NextDouble());
                row.Feature2 += (float)(noiseLevel * random.NextDouble());
            }

            Console.WriteLine("Gaussian noise injected into dataset.");
        }
    }
}
