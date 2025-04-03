using System;
using System.Collections.Generic;
using System.Linq;

namespace BlurMageddon
{
    /// <summary>
    /// ModelTrainer class handles machine learning model training and evaluation.
    /// It provides methods for setting up pipelines, training models, and analyzing results.
    /// </summary>
    public class ModelTrainer
    {
        /// <summary>
        /// Trains a machine learning model using the provided dataset.
        /// </summary>
        /// <param name="trainData">The training dataset as a List of DataRow objects.</param>
        /// <param name="labelColumnName">The name of the column containing the labels.</param>
        /// <returns>A basic mock "trained model" for demonstration purposes.</returns>
        public Func<DataRow, string> TrainModel(List<DataRow> trainData, string labelColumnName)
        {
            Console.WriteLine("Starting training process...");

            // Mock training logic: Calculate class distribution and "train" a simple decision rule
            var classCounts = trainData.GroupBy(row => row.Label)
                                       .ToDictionary(g => g.Key, g => g.Count());

            Console.WriteLine("Training complete. Class distribution:");
            foreach (var kvp in classCounts)
            {
                Console.WriteLine($"Label: {kvp.Key}, Count: {kvp.Value}");
            }

            // Return a simple prediction function based on the majority class
            string majorityClass = classCounts.OrderByDescending(kvp => kvp.Value).First().Key;
            Console.WriteLine($"Majority class is '{majorityClass}'. Using this as the prediction.");

            return (row) =>
            {
                // This simple function always predicts the majority class
                return majorityClass;
            };
        }

        /// <summary>
        /// Evaluates the performance of a trained model using the test dataset.
        /// </summary>
        /// <param name="model">The trained model (a function that predicts a label).</param>
        /// <param name="testData">The test dataset as a List of DataRow objects.</param>
        public void EvaluateModel(Func<DataRow, string> model, List<DataRow> testData)
        {
            Console.WriteLine("Evaluating model...");

            // Count total and correctly predicted rows
            int correctPredictions = 0;
            foreach (var row in testData)
            {
                string predictedLabel = model(row);
                if (predictedLabel == row.Label)
                {
                    correctPredictions++;
                }
            }

            // Calculate accuracy
            float accuracy = (float)correctPredictions / testData.Count;
            Console.WriteLine($"Evaluation complete. Accuracy: {accuracy:P2}");
        }
    }
}
