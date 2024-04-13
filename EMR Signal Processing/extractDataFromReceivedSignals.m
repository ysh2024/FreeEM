% This program converts the received EMR signals into a spectrogram and extracts training and testing data from the spectrogram. 
% The training data is saved in four files, which are the available chips sent from four victim computers to the attacker respectively. 
% The testing data is saved in one file, which is combined chips sent from four victim computers to the attacker simultaneously.
% Input: comp.mat
% Output: MAabs.mat, MBabs.mat, MCabs.mat, MDabs.mat, rawTestData.mat
clear;
clc;
close all;

thres=0.5;% Weaken the high-energy signals on the spectrogram, making the weak EMR signals more prominent on the spectrogram.
fftSize=1024; % Frequency resolution (width) of the spectrogram.
SP=13586+25019;% Starting point of training data

% Read the received signals and construct them into a matrix..
if exist('comp.mat','file')==0
    path=[pwd,'\EMR.dat'];
    id = fopen(path, 'r');
    Data=fread(id,'float32');
    fclose(id);
    R=Data(1:2:end-1);
    I=Data(2:2:end);
    comp=single(R+I*1i).';
    save comp.mat comp;
else
    load comp.mat;
end

row=floor(length(comp)/fftSize);
M=reshape(comp(1:row*fftSize),fftSize,row);
M=M.';

% Convert time-domain signals into frequency-domain signals. Due to the limited amount of data that can be processed by the GPU, it needs to be converted in batches.
block=100000;
for row=1:block:size(M,1)
    m=M(row:min([row+block-1,size(M,1)]),:);
    m=gpuArray(m);
    m=fftshift(fft(m,fftSize,2),2);
    m=gather(m);
    M(row:min([row+block-1,size(M,1)]),:)=m;
end

M=abs(M);
M(M>thres)=thres;
figure('Name','Spectrum of EMR Signals');
imagesc(M);hold on;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract testing data from the spectrogram and save it as rawTestData.mat.
startData=SP-25019;
finishData=SP-21;
rawTestData=M(startData:finishData,:);
save rawTestData.mat rawTestData;
figure('Name','rawTestData');
imagesc(rawTestData);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract training data (sent from victim A) from the
% spectrogram and save it as MAabs.mat.
startA=SP;
finishA=SP+15624;
MA=M(startA:finishA,:);
save MAabs.mat MA;
figure('Name','Training Data from Sender A');
imagesc(MA);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract training data (sent from victim B) from the spectrogram
% and save it as MBabs.mat.
startB=SP+15625;
finishB=SP+31249;
MB=M(startB:finishB,:);
save MBabs.mat MB;
figure('Name','Training Data from Sender B');
imagesc(MB);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract training data (sent from victim C) from the spectrogram
% and save it as MCabs.mat.
startC=SP+31250;
finishC=SP+46874;
MC=M(startC:finishC,:);
save MCabs.mat MC;
figure('Name','Training Data from Sender C');
imagesc(MC);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract training data (sent from victim D) from the spectrogram
% and save it as MDabs.mat.
startD=SP+46875;
finishD=SP+62499;
MD=M(startD:finishD,:);
save MDabs.mat MD;
figure('Name','Training Data from Sender D');
imagesc(MD);hold on;
ylabel('time');
xlabel('frequency (MHz)');
