using System;
using System.Collections.Generic;

namespace BlurMageddon
{
    public class BlurChaos
    {
        public static void Run()
        {
            try
            {
                // Step 1: Load the dataset into memory using DataLoader
                string filePath = "datasets/sample.csv";
                var dataLoader = new DataLoader();
                var data = dataLoader.LoadData(filePath);
                Console.WriteLine($"Loaded data: {data.Count} rows.");

                // Step 2: Baseline Results (Clean Data) for Perceptron and SVM
                Console.WriteLine("Running Baseline Experiment (Clean Data)...");

                var baselineTrainTestSplit = SplitData(data, 0.3f);

                var perceptron = new Perceptron(); // Initialize Perceptron
                Console.WriteLine("Baseline Results for Perceptron:");
                RunExperiment(perceptron, baselineTrainTestSplit.TrainSet, baselineTrainTestSplit.TestSet);

                var svm = new SupportVectorMachine(); // Initialize SVM
                Console.WriteLine("Baseline Results for SVM:");
                RunExperiment(svm, baselineTrainTestSplit.TrainSet, baselineTrainTestSplit.TestSet);

                // Step 3: Inject Noise into the Dataset
                Console.WriteLine("Injecting Noise into the Dataset...");
                float noiseLevel = 0.5f; // Adjust this value for desired noise intensity
                NoiseInjector.InjectGaussianNoise(data, noiseLevel);

                // Debug noisy data
                Console.WriteLine("Preview of noisy data (first 10 rows):");
                for (int i = 0; i < Math.Min(data.Count, 10); i++)
                {
                    Console.WriteLine($"Noisy Row {i}: Feature1 = {data[i].Feature1}, Feature2 = {data[i].Feature2}, Label = {data[i].Label}");
                }

                // Step 4: Post-Noise Results for Perceptron and SVM
                Console.WriteLine("Running Experiment (Noisy Data)...");

                var noisyTrainTestSplit = SplitData(data, 0.3f);

                Console.WriteLine("Post-Noise Results for Perceptron:");
                RunExperiment(perceptron, noisyTrainTestSplit.TrainSet, noisyTrainTestSplit.TestSet);

                Console.WriteLine("Post-Noise Results for SVM:");
                RunExperiment(svm, noisyTrainTestSplit.TrainSet, noisyTrainTestSplit.TestSet);

                // Step 5: Generate Visualization Plots
                Console.WriteLine("Generating Visualization Plots...");
                Visualization.GeneratePlots(baselineTrainTestSplit.TrainSet, noisyTrainTestSplit.TrainSet, perceptron, svm);
                Console.WriteLine("Plots saved to the 'plots' folder.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred during processing: {ex.Message}");
            }
        }

        /// <summary>
        /// Runs an experiment with a given model, training and evaluating it.
        /// </summary>
        private static void RunExperiment(IModel model, List<DataRow> trainData, List<DataRow> testData)
        {
            Console.WriteLine($"Training model: {model.GetType().Name}");
            model.Train(trainData); // Train the model
            Console.WriteLine($"Evaluating model: {model.GetType().Name}");
            model.Evaluate(testData); // Evaluate the model
        }

        /// <summary>
        /// Splits the dataset into training and testing sets based on the given test fraction.
        /// </summary>
        private static (List<DataRow> TrainSet, List<DataRow> TestSet) SplitData(List<DataRow> data, float testFraction)
        {
            var random = new Random();
            var shuffledData = new List<DataRow>(data);
            shuffledData.Sort((a, b) => random.Next(-1, 2)); // Randomly shuffle the data

            int testCount = (int)(shuffledData.Count * testFraction);
            var testSet = shuffledData.GetRange(0, testCount); // First part as TestSet
            var trainSet = shuffledData.GetRange(testCount, shuffledData.Count - testCount); // Remaining as TrainSet

            Console.WriteLine($"Split data into TrainSet: {trainSet.Count} rows, TestSet: {testSet.Count} rows.");
            return (trainSet, testSet);
        }
    }
}
