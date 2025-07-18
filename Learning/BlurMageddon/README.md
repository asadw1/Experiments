# BlurMageddon

**BlurMageddon** explores how various types of noise injection impact the performance and reliability of linear classification models, specifically the Perceptron and Support Vector Machine (SVM). The project highlights the limitations of these models under real-world data distortions.

## What It Does
- Evaluates the Perceptron and SVM on clean datasets as a baseline.
- Applies different noise injection techniques to disrupt the dataset.
- Analyzes how noise impedes the models' ability to classify data correctly.
- Visualizes the data transformations and decision boundary shifts caused by noise.

## Models
1. **Perceptron**: A linear classifier that relies on weight updates to separate data points.
2. **Support Vector Machine (SVM)**: A linear SVM that uses a hyperplane to separate classes.

## Noise Types
1. **Gaussian Noise**: Introduces random noise drawn from a normal distribution, simulating natural variability.
2. **Label Noise**: Random flipping of labels, creating classification errors.
3. **Random Feature Distortion**: Adds chaotic changes to feature values through additive and multiplicative transformations.
4. **Non-linear Transformations**: Distorts features using hyperbolic conic sections and inverse tangent mappings.
5. **Cholesky Decomposition**: Processes feature vectors using covariance matrix decomposition, introducing correlations and complex distortions.

# Project Structure

## Root Directory: `BlurMageddon`
This directory contains all files and subdirectories for the project.

### Files
- **BlurMageddon.sln**: Solution file for the project.
- **BlurMageddon.csproj**: Project file, specifying build settings and dependencies.
- **Program.cs**: Main entry point for the application.
- **README.md**: Project documentation and overview.

### Folders
- **plots\\**: Generated plots and visualizations of datasets and model results.
- **datasets\\**: Contains input datasets, typically CSV files, used for experiments.
- **src\\**: Holds all source code files, including models, noise injection utilities, and visualization logic.
- **tests\\**: Contains unit and integration tests to verify project functionality.


---

Updated README on 2025-05-08 18:00:03.922299
Updated README on 2025-05-09 12:00:03.645522
Updated README on 2025-05-10 12:00:03.225789
Updated README on 2025-05-19 12:00:08.323993
Updated README on 2025-05-20 12:00:03.419402
Updated README on 2025-05-21 12:00:04.118617
Updated README on 2025-05-23 12:00:08.137386
Updated README on 2025-05-24 12:00:03.117554
Updated README on 2025-05-27 12:00:03.724410
Updated README on 2025-05-27 18:00:01.814132
Updated README on 2025-05-29 12:00:08.552642
Updated README on 2025-06-02 18:00:02.170417
Updated README on 2025-06-10 12:00:04.053622
Updated README on 2025-06-10 18:00:03.144445
Updated README on 2025-06-14 18:00:04.546368
Updated README on 2025-06-23 18:00:01.981539
Updated README on 2025-06-25 18:00:02.129322
Updated README on 2025-07-05 12:00:04.895978
Updated README on 2025-07-08 12:00:04.658203
Updated README on 2025-07-09 18:00:02.025406
Updated README on 2025-07-10 18:00:02.092912
Updated README on 2025-07-14 12:00:04.817876