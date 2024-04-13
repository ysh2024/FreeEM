% This program converts four chip sequences into four symbol sequences, and then calculates the accuracy of each symbol sequence.
% Input: Table.mat, stdTable.mat
% Output: symbolAccuracy & bitAccuracy
clear;
clc;
close all;
% The chip sequence that each victim computer wants to transmit (i.e., the correct answer).
A=[
    1,1,1,1,1,1,1,1;
    1,2,2,2,2,2,2,2;
    1,3,3,3,3,3,3,3;
    1,4,4,4,4,4,4,4;
    1,5,5,5,5,5,5,5;
    1,6,6,6,6,6,6,6;
    1,7,7,7,7,7,7,7;
    1,8,8,8,8,8,8,8;
    2,1,2,3,4,5,6,7;
    2,2,1,4,3,6,5,8;
    2,3,4,1,2,7,8,5;
    2,4,3,2,1,8,7,6;
    2,5,6,7,8,1,2,3;
    2,6,5,8,7,2,1,4;
    2,7,8,5,6,3,4,1;
    2,8,7,6,5,4,3,2];
B=[
    3,1,3,5,7,4,2,8;
    3,2,4,6,8,3,1,7;
    3,3,1,7,5,2,4,6;
    3,4,2,8,6,1,3,5;
    3,5,7,1,3,8,6,4;
    3,6,8,2,4,7,5,3;
    3,7,5,3,1,6,8,2;
    3,8,6,4,2,5,7,1;
    4,1,4,7,6,8,5,2;
    4,2,3,8,5,7,6,1;
    4,3,2,5,8,6,7,4;
    4,4,1,6,7,5,8,3;
    4,5,8,3,2,4,1,6;
    4,6,7,4,1,3,2,5;
    4,7,6,1,4,2,3,8;
    4,8,5,2,3,1,4,7];
C=[
    5,1,5,4,8,7,3,6;
    5,2,6,3,7,8,4,5;
    5,3,7,2,6,5,1,8;
    5,4,8,1,5,6,2,7;
    5,5,1,8,4,3,7,2;
    5,6,2,7,3,4,8,1;
    5,7,3,6,2,1,5,4;
    5,8,4,5,1,2,6,3;
    6,1,6,2,5,3,8,4;
    6,2,5,1,6,4,7,3;
    6,3,8,4,7,1,6,2;
    6,4,7,3,8,2,5,1;
    6,5,2,6,1,7,4,8;
    6,6,1,5,2,8,3,7;
    6,7,4,8,3,5,2,6;
    6,8,3,7,4,6,1,5];

D=[
    7,1,7,8,2,6,4,3;
    7,2,8,7,1,5,3,4;
    7,3,5,6,4,8,2,1;
    7,4,6,5,3,7,1,2;
    7,5,3,4,6,2,8,7;
    7,6,4,3,5,1,7,8;
    7,7,1,2,8,4,6,5;
    7,8,2,1,7,3,5,6;
    8,1,8,6,3,2,7,5;
    8,2,7,5,4,1,8,6;
    8,3,6,8,1,4,5,7;
    8,4,5,7,2,3,6,8;
    8,5,4,2,7,6,3,1;
    8,6,3,1,8,5,4,2;
    8,7,2,4,5,8,1,3;
    8,8,1,3,6,7,2,4];

load Table.mat;
load stdTable.mat;

startCol=1;
finishCol=8;

% calculate the symbol accuracy of sender A
chipSeq=[];
for th=1:1:10
    chipSeq=[chipSeq;Table(th:10:size(Table,1),1)];%s1,s2,...,s16, s1,s2,...,s16, 10 times (160 symbols) in total
end
sbMat=reshape(chipSeq,8,1600)';
sbSeq=[];
for row=1:1:size(sbMat,1)
    sb=sbMat(row,[startCol:finishCol]);
    eqMax=0;
    for r=1:1:16
        eq=sum(sb==A(r,[startCol:finishCol]));
        if eq>eqMax
            eqMax=eq;
            SB=r;
        end
    end
    sbSeq=[sbSeq,SB];
end
stdSbSeq=repmat(1:16,1,100);
stdSbSeqBinary=repmat(0:15,1,100);
symbolAccuracy=sum(sbSeq==stdSbSeq)/1600
box=[];
for cnt=1:1:10
    box=[box sum(sbSeq(160*(cnt-1)+1:160*cnt)==stdSbSeq(160*(cnt-1)+1:160*cnt))/160];
end

stdBit=dec2bin(stdSbSeqBinary,4);
bit=dec2bin(sbSeq-1,4);
bitAccuracy=sum(bit==stdBit,'all')/(1600*4)

% calculate the symbol accuracy of sender B
chipSeq=[];
for th=1:1:10
    chipSeq=[chipSeq;Table(th:10:size(Table,1),2)];%s1,s2,...,s16, s1,s2,...,s16, 10 times (160 symbols) in total
end
sbMat=reshape(chipSeq,8,1600)';
sbSeq=[];
for row=1:1:size(sbMat,1)
    sb=sbMat(row,[startCol:finishCol]);
%     if row==15
%         sb
%     end
    eqMax=0;
    for r=1:1:16
        eq=sum(sb==B(r,[startCol:finishCol]));
        if eq>eqMax
            eqMax=eq;
            SB=r;
        end
    end
    sbSeq=[sbSeq,SB];
end
symbolAccuracy=sum(sbSeq==stdSbSeq)/1600
box=[];
for cnt=1:1:10
    box=[box sum(sbSeq(160*(cnt-1)+1:160*cnt)==stdSbSeq(160*(cnt-1)+1:160*cnt))/160];
end

stdBit=dec2bin(stdSbSeqBinary,4);
bit=dec2bin(sbSeq-1,4);
bitAccuracy=sum(bit==stdBit,'all')/(1600*4)


% calculate the symbol accuracy of sender C
chipSeq=[];
for th=1:1:10
    chipSeq=[chipSeq;Table(th:10:size(Table,1),3)];%s1,s2,...,s16, s1,s2,...,s16, 10 times (160 symbols) in total
end
sbMat=reshape(chipSeq,8,1600)';
sbSeq=[];
for row=1:1:size(sbMat,1)
    sb=sbMat(row,[startCol:finishCol]);
    eqMax=0;
    for r=1:1:16
        eq=sum(sb==C(r,[startCol:finishCol]));
        if eq>eqMax
            eqMax=eq;
            SB=r;
        end
    end
    sbSeq=[sbSeq,SB];
end
symbolAccuracy=sum(sbSeq==stdSbSeq)/1600
box=[];
for cnt=1:1:10
    box=[box sum(sbSeq(160*(cnt-1)+1:160*cnt)==stdSbSeq(160*(cnt-1)+1:160*cnt))/160];
end

stdBit=dec2bin(stdSbSeqBinary,4);
bit=dec2bin(sbSeq-1,4);
bitAccuracy=sum(bit==stdBit,'all')/(1600*4)


% calculate the symbol accuracy of sender D
chipSeq=[];
for th=1:1:10
    chipSeq=[chipSeq;Table(th:10:size(Table,1),4)];%s1,s2,...,s16, s1,s2,...,s16, 10 times (160 symbols) in total
end
sbMat=reshape(chipSeq,8,1600)';
sbSeq=[];
for row=1:1:size(sbMat,1)
    sb=sbMat(row,[startCol:finishCol]);
    eqMax=0;
    for r=1:1:16
        eq=sum(sb==D(r,[startCol:finishCol]));
        if eq>eqMax
            eqMax=eq;
            SB=r;
        end
    end
    sbSeq=[sbSeq,SB];
end
symbolAccuracy=sum(sbSeq==stdSbSeq)/1600
box=[];
for cnt=1:1:10
    box=[box sum(sbSeq(160*(cnt-1)+1:160*cnt)==stdSbSeq(160*(cnt-1)+1:160*cnt))/160];
end

stdBit=dec2bin(stdSbSeqBinary,4);
bit=dec2bin(sbSeq-1,4);
bitAccuracy=sum(bit==stdBit,'all')/(1600*4)


