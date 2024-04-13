1. generateEMR.c needs to be executed on a Linux system.
2. generate2DSymbols.m needs to be run with MATLAB.

3. How to run the programs in the folder: EMR Signal Processing?

3.1 Download all the programs to a local folder and run them using MATLAB. Before running the program, you need to download the raw data (comp.mat) and place it in the same directory as the .m program. Raw data can be downloaded from the following link:

https://drive.google.com/file/d/1fcsYbMnhJUFGZkLRDSpGAILPjI3GcCv-/view?usp=sharing

Then, proceed to run the following programs sequentially.

3.2 Run the "extractDataFromReceivedSignals.m" to convert the received EMR signals into a spectrogram and extract training and testing data from the spectrogram.

    Input: comp.mat
    
    Output: MAabs.mat, MBabs.mat, MCabs.mat, MDabs.mat, rawTestData.mat

3.3 Run the "constructTrainingData.m" to utilize the chips received from each victim computer to construct combined chips (and corresponding labels), which will be used for training the neural network in subsequent processes.

    Input: MAabs.mat, MBabs.mat, MCabs.mat, MDabs.mat
    
    Output: TrainData.mat, TrainLabel.mat, DataCompletionForDNN.mat

3.4 Run the "constructTestingData.m" to constructs test data and assigns correct labels to them. Later, a neural network will classify the test data, comparing its predictions with the correct labels to calculate accuracy. 

    Input: rawTestData.mat, DataCompletionForDNN.mat
    
    Output: TestData.mat, TestLabel.mat

3.5 Run the "classify.m" to train the DNN using training data, tests the DNN using test data, and compares the resulting classification with the test labels to calculate the classification accuracy.

    Input: TrainData.mat, TrainLabel.mat, TestData.mat, TestLabel.mat
    
    Output: Table.mat, stdTable.mat

3.6 Run the "chipToSymbol.m" to convert chip sequences into symbol sequences, and then calculates the accuracy of each symbol sequence.

    Input: Table.mat, stdTable.mat
    
    Output: symbolAccuracy & bitAccuracy

