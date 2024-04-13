1. generateEMR.c needs to be executed on a Linux system.

2. How to run the programs in the folder: EMR Signal Processing?

2.1 To run the programs in the folder: EMR Signal Processing, raw data (comp.mat) is needed, which can be downloaded from the following link:

https://drive.google.com/file/d/1fcsYbMnhJUFGZkLRDSpGAILPjI3GcCv-/view?usp=sharing

Then, place the "comp.mat" in the same directory as the .m program; 

2.2 Run the "extractDataFromReceivedSignals.m" to convert the received EMR signals into a spectrogram and extract training and testing data from the spectrogram.

2.3 Run the "constructTrainingData.m" to utilize the chips received from each victim computer to construct combined chips (and corresponding labels), which will be used for training the neural network in subsequent processes.

2.4 Run the "constructTestingData.m" to constructs test data and assigns correct labels to them. Later, a neural network will classify the test data, comparing its predictions with the correct labels to calculate accuracy. 

2.5 Run the "classify.m" to train the DNN using training data, tests the DNN using test data, and compares the resulting classification with the test labels to calculate the classification accuracy.

2.6 Run the "chipToSymbol.m" to convert chip sequences into symbol sequences, and then calculates the accuracy of each symbol sequence.
