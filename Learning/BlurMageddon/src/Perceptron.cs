using System;
using System.Collections.Generic;
using System.Linq;

namespace BlurMageddon
{
    public class Perceptron : IModel
    {
        private double[] _weights; // Weights for the input features
        private double _bias; // Threshold (bias term)

        public Perceptron()
        {
            _weights = new double[2]; // Initialize weights for two features
            _bias = 0; // Initialize bias
        }

        /// <summary>
        /// Trains the perceptron using the training dataset.
        /// </summary>
        public void Train(List<DataRow> trainData)
        {
            Console.WriteLine("Training Perceptron...");

            // Convert labels to binary (e.g., ClassA = +1, ClassB = -1)
            var binaryData = trainData.Select(row => new
            {
                Features = new double[] { row.Feature1, row.Feature2 },
                Label = row.Label == "ClassA" ? 1 : -1
            }).ToList();

            // Training loop
            for (int epoch = 0; epoch < 1000; epoch++) // Default 1000 epochs
            {
                foreach (var point in binaryData)
                {
                    // Calculate the prediction
                    double prediction = DotProduct(_weights, point.Features) + _bias;

                    // Update weights and bias if the prediction is incorrect
                    if (point.Label * prediction <= 0) // Misclassified point
                    {
                        _weights[0] += 0.01 * point.Label * point.Features[0];
                        _weights[1] += 0.01 * point.Label * point.Features[1];
                        _bias += 0.01 * point.Label;
                    }
                }
            }

            Console.WriteLine("Perceptron training complete.");
        }

        /// <summary>
        /// Evaluates the perceptron on the test dataset.
        /// </summary>
        public void Evaluate(List<DataRow> testData)
        {
            Console.WriteLine("Evaluating Perceptron...");
            int correctPredictions = 0;

            foreach (var row in testData)
            {
                // Calculate prediction
                double[] features = { row.Feature1, row.Feature2 };
                double prediction = DotProduct(_weights, features) + _bias;

                // Determine predicted label
                string predictedLabel = prediction >= 0 ? "ClassA" : "ClassB";

                // Compare with actual label
                if (predictedLabel == row.Label)
                    correctPredictions++;
            }

            // Calculate accuracy
            float accuracy = (float)correctPredictions / testData.Count;
            Console.WriteLine($"Evaluation complete. Accuracy: {accuracy:P2}");
        }

        /// <summary>
        /// Predicts the label for a single data point.
        /// </summary>
        public string Predict(DataRow row)
        {
            double[] features = { row.Feature1, row.Feature2 };
            double prediction = DotProduct(_weights, features) + _bias;
            return prediction >= 0 ? "ClassA" : "ClassB";
        }

        /// <summary>
        /// Calculates the dot product of two vectors.
        /// </summary>
        private double DotProduct(double[] vector1, double[] vector2)
        {
            return vector1.Zip(vector2, (v1, v2) => v1 * v2).Sum();
        }
    }
}
