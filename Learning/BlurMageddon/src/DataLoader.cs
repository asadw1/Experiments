using System;
using System.Collections.Generic;
using System.IO;

namespace BlurMageddon
{
    // Represents each row of data with its features and label
    public class DataRow
    {
        public float Feature1 { get; set; } // Numeric feature #1
        public float Feature2 { get; set; } // Numeric feature #2
        public string Label { get; set; } = ""; // Default to empty string to avoid warnings
    }

    // Handles the loading of data into memory
    public class DataLoader
    {
        // Method to load data from a CSV file into a List<DataRow>
        public List<DataRow> LoadData(string filePath)
        {
            var data = new List<DataRow>(); // Initialize an empty list to hold data rows

            Console.WriteLine($"Loading dataset from: {filePath}");

            // Verify the file exists
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"Dataset file not found at path: {filePath}");
            }

            // Read and parse the file line by line
            using (var reader = new StreamReader(filePath))
            {
                string? line; // Declare 'line' as nullable to match ReadLine() return type
                bool isHeader = true; // Flag to skip the header row

                while ((line = reader.ReadLine()) != null) // Loop through each line until EOF
                {
                    if (isHeader) // Skip the header row on the first iteration
                    {
                        isHeader = false;
                        continue;
                    }

                    var columns = line.Split(','); // Split the line into columns using the comma delimiter

                    // Validate the row and add to the list if valid
                    if (columns.Length == 3 &&
                        float.TryParse(columns[0], out var feature1) &&
                        float.TryParse(columns[1], out var feature2))
                    {
                        data.Add(new DataRow
                        {
                            Feature1 = feature1, // Assign Feature1 value
                            Feature2 = feature2, // Assign Feature2 value
                            Label = columns[2] ?? "" // Replace null values with an empty string
                        });
                    }
                    else
                    {
                        Console.WriteLine($"Invalid row: {line}"); // Log invalid rows for debugging
                    }
                }
            }

            Console.WriteLine($"Rows loaded: {data.Count}"); // Output the total number of valid rows loaded
            return data; // Return the list of data rows
        }
    }
}
