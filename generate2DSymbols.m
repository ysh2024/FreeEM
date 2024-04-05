clear;
clc;
dim=[8,8]; %[freq,time]
seqOfIndex=ones(1,dim(1)*dim(1));

candidateSet=0:1:power(dim(1),dim(2))-1;
coexistenceSet=[];
sizeOfCoexSet=0;
effectiveLenOfIndex=0;

reconstruct=0;
step=power(dim(1),dim(2))/length(seqOfIndex);
while sizeOfCoexSet<dim(1)*dim(1)
    
    if reconstruct==1
        coexistenceSet=[];
        candidateSet=0:1:power(dim(1),dim(2))-1;
        success=1;
        for cnt=1:1:sizeOfCoexSet
            if length(candidateSet)>=seqOfIndex(cnt)
                coexistenceSet=[coexistenceSet,candidateSet(seqOfIndex(cnt))];
                candidateSet(1:seqOfIndex(cnt))=[];
                cmp=coexistenceSet(end)*ones(1,length(candidateSet));
                del=sum(dec2base(candidateSet,dim(1),dim(2))==dec2base(cmp,dim(1),dim(2)),2)>1;
                candidateSet(del)=[];
            else
                success=0;
                break;
            end
        end
        if success==0
            seqOfIndex(cnt:end)=1;
            seqOfIndex(cnt-1)=seqOfIndex(cnt-1)+1;
            sizeOfCoexSet=cnt-2;
        else
            reconstruct=0;
        end
        
    end
    
    if reconstruct==0
    
        if length(candidateSet)<seqOfIndex(sizeOfCoexSet+1)||candidateSet(seqOfIndex(sizeOfCoexSet+1))>(sizeOfCoexSet+1)*step-1
            seqOfIndex(sizeOfCoexSet+1:end)=1;
            seqOfIndex(sizeOfCoexSet)=seqOfIndex(sizeOfCoexSet)+1;
            sizeOfCoexSet=sizeOfCoexSet-1;
            reconstruct=1;
        else
            coexistenceSet=[coexistenceSet,candidateSet(seqOfIndex(sizeOfCoexSet+1))];
            candidateSet(1:seqOfIndex(sizeOfCoexSet+1))=[];
            cmp=coexistenceSet(end)*ones(1,length(candidateSet));
            del=sum(dec2base(candidateSet,dim(1),dim(2))==dec2base(cmp,dim(1),dim(2)),2)>1;
            candidateSet(del)=[];

            if length(coexistenceSet)>effectiveLenOfIndex
                effectiveLenOfIndex=length(coexistenceSet);
                seqOfIndex;
                coexistenceSet=coexistenceSet;
            end
            sizeOfCoexSet=sizeOfCoexSet+1;
        end
    end
end

dec2base(coexistenceSet,dim(1),dim(2))
save coexistenceSet.mat coexistenceSet 