clear;
clc;
close all;

thres=0.5;
winSize=1;
loss=0.5;
fftSize=1024;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SP=13586+25019;% starting point of training data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

block=100000;% divide into several blocks to fit the limitation of GPU
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
startData=SP-25019;%from 2ms
finishData=SP-21;%to 1281ms
rawTestData=M(startData:finishData,:);
% distri=sum(rawTestData,1);
% rawTestData(:,distri/distri(512)>0.6)=0;
% distri=movmean(distri,winSize);
% distri=-rescale(distri,0,loss)+1;
% figure;
% plot(distri);
% rawTestData=rawTestData.*distri;
save rawTestData.mat rawTestData;
figure('Name','rawTestData');
imagesc(rawTestData);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
startA=SP;%1 chip lasts for 51.2us, 1 test chip set lasts for 1ms.
finishA=SP+15624;
MA=M(startA:finishA,:);
% distri=sum(MA,1);
% MA(:,distri/distri(512)>0.6)=0;
% distri=movmean(distri,winSize);
% distri=-rescale(distri,0,loss)+1;
% figure;
% plot(distri);
% figure;
% imagesc(MA);hold on;
% MA=MA.*distri;
save MAabs.mat MA;
figure('Name','Training Data from Sender A');
imagesc(MA);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
startB=SP+15625;%1 chip lasts for 51.2us, 1 training chip set lasts for 100ms.
finishB=SP+31249;
MB=M(startB:finishB,:);
% distri=sum(MB,1);
% MB(:,distri/distri(512)>0.6)=0;
% distri=movmean(distri,winSize);
% distri=-rescale(distri,0,loss)+1;
% figure;
% plot(distri);
% MB=MB.*distri;
save MBabs.mat MB;
figure('Name','Training Data from Sender B');
imagesc(MB);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
startC=SP+31250;
finishC=SP+46874;
MC=M(startC:finishC,:);
% distri=sum(MC,1);
% MC(:,distri/distri(512)>0.6)=0;
% distri=movmean(distri,winSize);
% distri=-rescale(distri,0,loss)+1;
% figure;
% plot(distri);
% MC=MC.*distri;
save MCabs.mat MC;
figure('Name','Training Data from Sender C');
imagesc(MC);hold on;
ylabel('time');
xlabel('frequency (MHz)');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
startD=SP+46875;
finishD=SP+62499;
MD=M(startD:finishD,:);
% distri=sum(MC,1);
% MC(:,distri/distri(512)>0.6)=0;
% distri=movmean(distri,winSize);
% distri=-rescale(distri,0,loss)+1;
% figure;
% plot(distri);
% MC=MC.*distri;
save MDabs.mat MD;
figure('Name','Training Data from Sender D');
imagesc(MD);hold on;
ylabel('time');
xlabel('frequency (MHz)');
