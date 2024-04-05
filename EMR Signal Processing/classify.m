clear;
clc;
close all;
load TrainData.mat;
load TrainLabel.mat;
load TestData.mat;
load TestLabel.mat

TrainData=abs(TrainData);
TestData=abs(TestData);


fftsize=1024;
chipSeqLen=1280;

% figure('Name','TrainData');
% imagesc(TrainData);hold on;
% 
% figure('Name','TestData');
% imagesc(TestData);hold on;

fc_layer = [
%     imageInputLayer([fftsize 1])
    featureInputLayer(fftsize, "Name", "input")

    fullyConnectedLayer(1024, "Name", "fc-1")
%     dropoutLayer(0.5,"Name","dropout")

    fullyConnectedLayer(2048, "Name", "fc-2")
%     dropoutLayer(0.5,"Name","dropout")

    fullyConnectedLayer(4096, "Name", "fc-3")
%     dropoutLayer(0.5,"Name","dropout")

    softmaxLayer("Name", "softmax")
    classificationLayer("Name", "classification")
    ];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
miniBatchSize = ceil(size(TrainData,1)/100);

options = trainingOptions('adam', ...
    'ExecutionEnvironment','gpu', ...
    'MaxEpochs',10, ...
    'MiniBatchSize',miniBatchSize, ...
    'ValidationData',{TestData,TestLabel}, ...
    'GradientThreshold',Inf, ...
    'Shuffle','every-epoch', ...
    'Verbose',false, ...
    'Plots','training-progress');

net = trainNetwork(TrainData, TrainLabel, fc_layer, options);
save('myNet.mat', 'net');

%%=========================================================================

load('myNet.mat', 'net');
[YPred prob] = classify(net, TestData, 'MiniBatchSize', miniBatchSize);

PredLabel=single(YPred(1:chipSeqLen*10,1));


figure;
plot(PredLabel,'b+');hold on;
plot(single(TestLabel(1:length(PredLabel))),'ro');hold on;
ylabel('label');
xlabel('received chips');
legend('predicted label','actual label');
% sum(PredLabel==single(TestLabel(1:chipSeqLen*10))')/chipSeqLen*10

save PredLabel.mat PredLabel;


%%=========================================================================
load PredLabel.mat;
stdLabel=single(TestLabel(1:chipSeqLen*10)');
Table=zeros(length(PredLabel),4);
stdTable=zeros(length(PredLabel),4);
for row=1:1:length(PredLabel)
    A=ceil(stdLabel(row)/8^3);
    B=ceil((stdLabel(row)-(A-1)*8^3)/8^2);
    C=ceil((stdLabel(row)-(A-1)*8^3-(B-1)*8^2)/8);
    D=ceil(stdLabel(row)-(A-1)*8^3-(B-1)*8^2-(C-1)*8);
    a=ceil(PredLabel(row)/8^3);
    b=ceil((PredLabel(row)-(a-1)*8^3)/8^2);
    c=ceil((PredLabel(row)-(a-1)*8^3-(b-1)*8^2)/8);
    d=ceil(PredLabel(row)-(a-1)*8^3-(b-1)*8^2-(c-1)*8);
    stdTable(row,:)=[A,B,C,D];
    Table(row,:)=[a,b,c,d];
end

%chip recognition accuracy for each sender
mean(Table(:,1) == stdTable(:,1))
mean(Table(:,2) == stdTable(:,2))
mean(Table(:,3) == stdTable(:,3))
mean(Table(:,4) == stdTable(:,4))
save Table.mat Table;
save stdTable.mat stdTable;
