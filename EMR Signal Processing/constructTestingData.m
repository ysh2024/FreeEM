clear;
clc;
close all;

chipSeqLen=1280;
startOffset=1;

load rawTestData.mat;
load DataCompletionForDNN.mat;
M=rawTestData;
% figure (1);
% imagesc(M);hold on;

array=zeros(chipSeqLen,10);

intvl=size(M,1)/chipSeqLen;
array(:,1)=round(startOffset:intvl:size(M,1))';

for cnt=2:1:10
    array(:,cnt)=array(:,1)+(cnt-1);
end

array=reshape(array.',1,chipSeqLen*10);

TestData=M(array,:);
%%=========================================================================
% DNN requires the whole 4096 types of labels, whereas testing data may
% not include all types, thus we append 4096 different labels (& testing data) to ensure
% that all types of labels appear.
TestData=[TestData;DataCompletionForDNN];
%%=========================================================================
% TestData(TestData>thres)=thres;
save TestData.mat TestData;

% data sent from 4 senders (to create testing labels & calculating SER)
data1=[1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,1,4,4,4,4,4,4,4,1,5,5,5,5,5,5,5,1,6,6,6,6,6,6,6,1,7,7,7,7,7,7,7,1,8,8,8,8,8,8,8,2,1,2,3,4,5,6,7,2,2,1,4,3,6,5,8,2,3,4,1,2,7,8,5,2,4,3,2,1,8,7,6,2,5,6,7,8,1,2,3,2,6,5,8,7,2,1,4,2,7,8,5,6,3,4,1,2,8,7,6,5,4,3,2];
data2=[3,1,3,5,7,4,2,8,3,2,4,6,8,3,1,7,3,3,1,7,5,2,4,6,3,4,2,8,6,1,3,5,3,5,7,1,3,8,6,4,3,6,8,2,4,7,5,3,3,7,5,3,1,6,8,2,3,8,6,4,2,5,7,1,4,1,4,7,6,8,5,2,4,2,3,8,5,7,6,1,4,3,2,5,8,6,7,4,4,4,1,6,7,5,8,3,4,5,8,3,2,4,1,6,4,6,7,4,1,3,2,5,4,7,6,1,4,2,3,8,4,8,5,2,3,1,4,7];
data3=[5,1,5,4,8,7,3,6,5,2,6,3,7,8,4,5,5,3,7,2,6,5,1,8,5,4,8,1,5,6,2,7,5,5,1,8,4,3,7,2,5,6,2,7,3,4,8,1,5,7,3,6,2,1,5,4,5,8,4,5,1,2,6,3,6,1,6,2,5,3,8,4,6,2,5,1,6,4,7,3,6,3,8,4,7,1,6,2,6,4,7,3,8,2,5,1,6,5,2,6,1,7,4,8,6,6,1,5,2,8,3,7,6,7,4,8,3,5,2,6,6,8,3,7,4,6,1,5];
data4=[7,1,7,8,2,6,4,3,7,2,8,7,1,5,3,4,7,3,5,6,4,8,2,1,7,4,6,5,3,7,1,2,7,5,3,4,6,2,8,7,7,6,4,3,5,1,7,8,7,7,1,2,8,4,6,5,7,8,2,1,7,3,5,6,8,1,8,6,3,2,7,5,8,2,7,5,4,1,8,6,8,3,6,8,1,4,5,7,8,4,5,7,2,3,6,8,8,5,4,2,7,6,3,1,8,6,3,1,8,5,4,2,8,7,2,4,5,8,1,3,8,8,1,3,6,7,2,4];

data1=repmat(data1,1,10);
data2=repmat(data2,1,10);
data3=repmat(data3,1,10);
data4=repmat(data4,1,10);

crctOvlpChip=(data1-1)*8^3+(data2-1)*8^2+(data3-1)*8+data4;
% figure;
% plot(crctOvlpChip,'+');

crctOvlpChip=repmat(crctOvlpChip,10,1);
crctOvlpChip=reshape(crctOvlpChip,1,chipSeqLen*10);

%%=========================================================================
% DNN requires the whole 4096 types of labels, whereas testing data may
% not include all types, thus we append 4096 different labels to ensure
% that all types of labels appear.
TestLabel=[crctOvlpChip,1:8^4];
%%=========================================================================
TestLabel=categorical(TestLabel);
save TestLabel.mat TestLabel;
