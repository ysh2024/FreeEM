clear;
clc;
close all;

fftSize=1024;
startOffset=100;% to avoid the RAM hasn't started
qty=200;% quantity of training data of each chip


load MAabs.mat;
load MBabs.mat;
load MCabs.mat;
load MDabs.mat;

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

% figure;
% plot(TrainLabel);hold on;

% TrainData(TrainData>thres)=thres;
TrainLabel=categorical(TrainLabel);

tic;
savefast TrainData.mat TrainData;%save 240s=4min
toc
save TrainLabel.mat TrainLabel;

figure('Name','Combined Chips for Training');
imagesc(TrainData);hold on;
size(TrainData)
DataCompletionForDNN=TrainData(1:200:end,:);
save DataCompletionForDNN.mat DataCompletionForDNN;

function [data,label]=Combine(data1,lb1,data2,lb2,data3,lb3,data4,lb4,qty)
    label=ones(qty,1)*((lb1-1)*8^3+(lb2-1)*8^2+(lb3-1)*8+lb4);
    cube(:,:,1)=data1(1:qty,:);
    cube(:,:,2)=data2(1:qty,:);
    cube(:,:,3)=data3(1:qty,:);
    cube(:,:,4)=data4(1:qty,:);
    data=max(cube,[],3);
end




