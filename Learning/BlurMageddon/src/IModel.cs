namespace BlurMageddon
{
    public interface IModel
    {
        void Train(List<DataRow> trainData);
        void Evaluate(List<DataRow> testData);
        string Predict(DataRow row); // New method for predictions
    }

}
