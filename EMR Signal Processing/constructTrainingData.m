% This program utilizes the chips received from each victim computer
% to construct combined chips (and corresponding labels), which will be used for training the neural network in subsequent processes.
% Input: MAabs.mat, MBabs.mat, MCabs.mat, MDabs.mat
% Output: TrainData.mat, TrainLabel.mat, DataCompletionForDNN.mat

clear;
clc;
close all;

fftSize=1024;
startOffset=100;% Due to the victim's unstable initiation of EMR signal transmission, we discard some unstable signals at the beginning when processing the received signals.
qty=200;% The victim will send the same chip multiple times to provide a sufficient amount of training data. We will extract $qty$ of these for training purposes.


load MAabs.mat;
load MBabs.mat;
load MCabs.mat;
load MDabs.mat;

% Determine the starting and ending points for extracting the signals.
intvl=size(MA,1)/8;
startA=round(startOffset:intvl:size(MA,1));
startA=startA(1:8);
startB=startA;
startC=startA;
startD=startA;

finishA=startA+5*(qty-1);
finishB=finishA;
finishC=finishA;
finishD=finishA;

TrainData=single(zeros(qty*(8^4),fftSize));
TrainLabel=single(zeros(qty*(8^4),1));

% Each victim computer can send 8 different chips. At any given time, there could be 8^4 different combinations of chips from the 4 victim computers.
% Hence, we utilize a single chip from each victim computer to construct 8^4 combinations for training the neural network.
type=1;
for A=1:1:8
    A
    for B=1:1:8
        B
        for C=1:1:8
            for D=1:1:8
                [Data,Label]=Combine(MA(startA(A):5:finishA(A),:),A,MB(startB(B):5:finishB(B),:),B,MC(startC(C):5:finishC(C),:),C,MD(startD(D):5:finishD(D),:),D,qty);
                type=(A-1)*8^3+(B-1)*8^2+(C-1)*8+D;
                TrainData((type-1)*qty+1:type*qty,:)=Data;
                TrainLabel((type-1)*qty+1:type*qty,1)=Label;
            end
        end
    end
end

TrainLabel=categorical(TrainLabel);

tic;
savefast TrainData.mat TrainData;
toc
save TrainLabel.mat TrainLabel;

% During testing, the adopted neural network needs all 8^4 combinations, but received data may lack some. We manually add missing combinations to the testing data, ensuring smooth operation. These additions aren't considered in error rate calculations.
figure('Name','Combined Chips for Training');
imagesc(TrainData);hold on;
size(TrainData)
DataCompletionForDNN=TrainData(1:200:end,:);
save DataCompletionForDNN.mat DataCompletionForDNN;

% Overlay chips from individual victim computers to create combined chips
function [data,label]=Combine(data1,lb1,data2,lb2,data3,lb3,data4,lb4,qty)
    label=ones(qty,1)*((lb1-1)*8^3+(lb2-1)*8^2+(lb3-1)*8+lb4);
    cube(:,:,1)=data1(1:qty,:);
    cube(:,:,2)=data2(1:qty,:);
    cube(:,:,3)=data3(1:qty,:);
    cube(:,:,4)=data4(1:qty,:);
    data=max(cube,[],3);
end




