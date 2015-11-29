NumberOfNodes = 1000000;
DataRange = floor(NumberOfNodes/0.75);

A = randperm(DataRange);
B = randperm(DataRange);

number_of_nodes_1 = ceil(0.5 * DataRange); %taking half the set

Ai = zeros(1,number_of_nodes_1); % node set for insert()
Bi = zeros(1,number_of_nodes_1); % node set for search_then_insert()

for i = 1:number_of_nodes_1
    Ai(i) = A(i); 
    Bi(i) = B(i);
end 

% set of keys being inserted
union_AiBi = union(Ai,Bi);
% just checking how many has been search_then NOT inserted
inters_AiBi = intersect(Ai,Bi);
% just checking how many has been search_then_inserted
diff_AiBi = setdiff(Bi, Ai);

% now let's look for numbers that haven't been inserted before
% so we can do the updates 
% update() ->> search for a key, and replace it with another key
% find nodes in A that's not in Ai and Bi
Set_NumbersLeft = setdiff(A, union_AiBi);

size_Ai = numel(Ai);
size_Bi = numel(Bi);
size_SNL = numel(Set_NumbersLeft);

Set2Replace = datasample(union_AiBi,size_SNL,'Replace',false );

Set_Update = zeros(1, 2*size_SNL);
for i = 1:size_SNL
    Set_Update(2*i - 1) = Set2Replace(i);
    Set_Update(2 *i) = Set_NumbersLeft(i);
end 

% --------- making the data sequence ----------
% insert() data 
% check_then_insert() data
% update() data

size_Set_Update = numel(Set_Update);

data = zeros(1, size_Ai + size_Bi + size_Set_Update);

for i = 1:size_Ai
    data(i) = Ai(i);
end

for i = (size_Ai + 1):(size_Ai + size_Bi)
    data(i) = Bi(i - size_Ai);
end

for i = (size_Ai + size_Bi + 1):(size_Ai + size_Bi + size_Set_Update)
    data(i) = Set_Update(i - size_Ai - size_Bi);
end 

Number_of_first_inserts = numel(Ai);
Number_of_searches = numel(Bi);
Number_of_inserts_after_searches = numel(diff_AiBi);
Number_of_pairs_for_updates = numel(Set_Update)/2;
total_nodes = Number_of_first_inserts + Number_of_inserts_after_searches;
perc_nodes_over_all = total_nodes/DataRange;
perc_nodes_second_insert = Number_of_inserts_after_searches/total_nodes;
perc_updates = Number_of_pairs_for_updates/total_nodes;

% good propotions would be [#nodes 0.75 0.3 0.4]
expected_total_nodes = NumberOfNodes;
total_nodes;
t = Number_of_first_inserts/total_nodes
analysis = [perc_nodes_over_all perc_nodes_second_insert  perc_updates]

if NumberOfNodes > 1000
    good_threshold = 0.001;
    if NumberOfNodes > 100000
        good_threshold = 0.0001;
    end
    
else
    good_threshold = 0.01;
end 


if abs(NumberOfNodes - total_nodes)/NumberOfNodes < good_threshold
    GOOD_TO_GO = 1;
else
    GOOD_TO_GO = 0;
end

GOOD_TO_GO

first_insert = number_of_nodes_1
check_insert = number_of_nodes_1

num_KB = numel(data) * 4/1024