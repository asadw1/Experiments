using System;
using System.Collections.Generic;
using System.IO;
using ScottPlot;

namespace BlurMageddon
{
    public class Visualization
    {
        public static void GeneratePlots(List<DataRow> cleanData, List<DataRow> noisyData, Perceptron perceptron, SupportVectorMachine svm)
        {
            string plotsFolder = "plots";

            // Ensure plots folder exists
            if (!Directory.Exists(plotsFolder))
            {
                Directory.CreateDirectory(plotsFolder);
            }

            // Define image dimensions
            int width = 800;
            int height = 600;

            // Plot 1: Clean Data with Perceptron Decision Boundary
            var pltCleanPerceptron = new ScottPlot.Plot();
            ScatterData(cleanData, pltCleanPerceptron, "Clean Data (Perceptron)");
            OverlayDecisionBoundary(cleanData, perceptron, pltCleanPerceptron);
            pltCleanPerceptron.SavePng(Path.Combine(plotsFolder, "clean_data_perceptron.png"), width, height);

            // Plot 2: Clean Data with SVM Decision Boundary
            var pltCleanSVM = new ScottPlot.Plot();
            ScatterData(cleanData, pltCleanSVM, "Clean Data (SVM)");
            OverlayDecisionBoundary(cleanData, svm, pltCleanSVM);
            pltCleanSVM.SavePng(Path.Combine(plotsFolder, "clean_data_svm.png"), width, height);

            // Plot 3: Noisy Data with Perceptron Decision Boundary
            var pltNoisyPerceptron = new ScottPlot.Plot();
            ScatterData(noisyData, pltNoisyPerceptron, "Noisy Data (Perceptron)");
            OverlayDecisionBoundary(noisyData, perceptron, pltNoisyPerceptron);
            pltNoisyPerceptron.SavePng(Path.Combine(plotsFolder, "noisy_data_perceptron.png"), width, height);

            // Plot 4: Noisy Data with SVM Decision Boundary
            var pltNoisySVM = new ScottPlot.Plot();
            ScatterData(noisyData, pltNoisySVM, "Noisy Data (SVM)");
            OverlayDecisionBoundary(noisyData, svm, pltNoisySVM);
            pltNoisySVM.SavePng(Path.Combine(plotsFolder, "noisy_data_svm.png"), width, height);
        }

        /// <summary>
        /// Plots the scatter data for ClassA and ClassB.
        /// </summary>
        private static void ScatterData(List<DataRow> data, ScottPlot.Plot plt, string title)
        {
            // Separate data into ClassA and ClassB for plotting
            var classA = data.FindAll(row => row.Label == "ClassA");
            var classB = data.FindAll(row => row.Label == "ClassB");

            double[] classAX = classA.ConvertAll(row => (double)row.Feature1).ToArray();
            double[] classAY = classA.ConvertAll(row => (double)row.Feature2).ToArray();
            double[] classBX = classB.ConvertAll(row => (double)row.Feature1).ToArray();
            double[] classBY = classB.ConvertAll(row => (double)row.Feature2).ToArray();

            // Add scatter plots for each class using ScottPlot.Color
            var scatterClassA = plt.Add.Scatter(classAX, classAY, color: new ScottPlot.Color(0, 0, 255)); // Blue
            scatterClassA.LegendText = "ClassA";

            var scatterClassB = plt.Add.Scatter(classBX, classBY, color: new ScottPlot.Color(255, 0, 0)); // Red
            scatterClassB.LegendText = "ClassB";

            // Add title and labels
            plt.Title(title);
            plt.XLabel("Feature1");
            plt.YLabel("Feature2");

            // Enable legend using ShowLegend()
            plt.ShowLegend();
        }

        /// <summary>
        /// Overlays the decision boundary of the given model onto the plot.
        /// </summary>
        private static void OverlayDecisionBoundary(List<DataRow> data, IModel model, ScottPlot.Plot plt)
        {
            double xMin = data.Min(row => row.Feature1);
            double xMax = data.Max(row => row.Feature1);
            double yMin = data.Min(row => row.Feature2);
            double yMax = data.Max(row => row.Feature2);

            // Generate decision boundary points
            List<double> boundaryX = new();
            List<double> boundaryY = new();

            for (double x = xMin; x <= xMax; x += 0.1)
            {
                for (double y = yMin; y <= yMax; y += 0.1)
                {
                    var tempRow = new DataRow { Feature1 = (float)x, Feature2 = (float)y };
                    if (model.Predict(tempRow) == "ClassA")
                    {
                        boundaryX.Add(x);
                        boundaryY.Add(y);
                    }
                }
            }

            var decisionBoundary = plt.Add.Scatter(boundaryX.ToArray(), boundaryY.ToArray(), color: new ScottPlot.Color(0, 255, 0)); // Green
            decisionBoundary.LegendText = "Decision Boundary";
        }
    }
}
