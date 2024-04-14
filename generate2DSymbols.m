clear;
clc;
dim=[8,8]; %[freq,time]
seqOfIndex=ones(1,dim(1)*dim(1));% index sequence. 
% It indicates which element to pick from the candidate set and place into 
% the coexistence set each time. 
% For example, seqOfIndex(2)=6 indicates that during the second selection 
% of elements, the 6th element from the candidate set needs to be moved 
% into the coexistence set.
effectiveLenOfIndex=0;% effective length of the index sequence
% It indicates which elements in the current index sequence are valid.
% For example, effectiveLenOfIndex=5 means the first five elements are
% valid. 

candidateSet=0:1:power(dim(1),dim(2))-1;
coexistenceSet=[];
sizeOfCoexSet=0; 

reconstruct=0;% whether need to reconstruct coexistenceSet
step=power(dim(1),dim(2))/length(seqOfIndex);
while sizeOfCoexSet<dim(1)*dim(1)
    
    if reconstruct==1
        % reconstruct coexistenceset according to current index sequence
        % and effective length of the index sequence
        coexistenceSet=[];
        candidateSet=0:1:power(dim(1),dim(2))-1;
        success=1;
        for cnt=1:1:sizeOfCoexSet
            if length(candidateSet)>=seqOfIndex(cnt)
                coexistenceSet=[coexistenceSet,candidateSet(seqOfIndex(cnt))];% Move a certain element from the candidate set into the coexistence set.
                candidateSet(1:seqOfIndex(cnt))=[];% Delete the element and those preceding it from the candidate set.
                cmp=coexistenceSet(end)*ones(1,length(candidateSet));
                del=sum(dec2base(candidateSet,dim(1),dim(2))==dec2base(cmp,dim(1),dim(2)),2)>1;% Find which elements in the candidate set cannot coexist with the element just selected.
                candidateSet(del)=[];% Delete the aforementioned elements that cannot coexist from the candidate set.
            else
                success=0;
                break;
            end
        end
        if success==0
            % If the reconstruction fails, it indicates that the selection method indicated by seqOfIndex is incorrect, and a new selection method needs to be chosen.
            seqOfIndex(cnt:end)=1;
            seqOfIndex(cnt-1)=seqOfIndex(cnt-1)+1;
            sizeOfCoexSet=cnt-2;
        else
            reconstruct=0;
        end
        
    end
    
    if reconstruct==0
    
        if length(candidateSet)<seqOfIndex(sizeOfCoexSet+1)||candidateSet(seqOfIndex(sizeOfCoexSet+1))>(sizeOfCoexSet+1)*step-1
            % there is no sufficient items in candidate set for selecting
            % according to the index sequence.
            seqOfIndex(sizeOfCoexSet+1:end)=1;
            seqOfIndex(sizeOfCoexSet)=seqOfIndex(sizeOfCoexSet)+1;% The previously selected element from the candidate set (suppose it's the x-th element) is deemed inappropriate. It should be returned to the candidate set, and the (x+1)-th element should be selected and placed into the coexistence set.
            sizeOfCoexSet=sizeOfCoexSet-1;% reduce the effective length of index sequence
            reconstruct=1;% reconstruct the coexistence set
        else
            % it is feasible to pick out item from candidate set
            coexistenceSet=[coexistenceSet,candidateSet(seqOfIndex(sizeOfCoexSet+1))];% Move a certain element from the candidate set into the coexistence set.
            candidateSet(1:seqOfIndex(sizeOfCoexSet+1))=[];% Delete the element and those preceding it from the candidate set.
            cmp=coexistenceSet(end)*ones(1,length(candidateSet));
            del=sum(dec2base(candidateSet,dim(1),dim(2))==dec2base(cmp,dim(1),dim(2)),2)>1;% Find which elements in the candidate set cannot coexist with the element just selected.
            candidateSet(del)=[];% Delete the aforementioned elements that cannot coexist from the candidate set.

            if length(coexistenceSet)>effectiveLenOfIndex
                effectiveLenOfIndex=length(coexistenceSet);
            end
            sizeOfCoexSet=sizeOfCoexSet+1;
        end
    end
end

dec2base(coexistenceSet,dim(1),dim(2))
save coexistenceSet.mat coexistenceSet 