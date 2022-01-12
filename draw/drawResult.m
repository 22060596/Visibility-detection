 clear;
close all;
mydata = importdata('zhoushan.txt');
for i = 1:length(mydata)
   temp = string(mydata(i));
   str = strsplit(temp, ',');
   data_temp = str2double(str(2));
   str2 = strsplit(str(1), '\');
   label_temp = str2double(strtok(str2(2), '_'));
   data(i) = data_temp;
   label(i) = label_temp;
end


% mydata = importdata('test2.txt');
% for i = 1:length(mydata.textdata)
%    temp = string(mydata.textdata(i));
%    str = strsplit(temp, '\');
%    label_temp = str2double(strtok(str(2), '_'));
%    label(i) = label_temp;
% end
% data = mydata.data;

count90 = 0;
count80 = 0;
for i = 1:length(label)
    if(data(i)<=1.2*label(i) && data(i)>=0.8*label(i))
        count80 = count80 + 1;
    end
    if(data(i)<=1.1*label(i) && data(i)>=0.9*label(i))
        count90 = count90 + 1;
    end
end
acc80 = count80 / length(label);
acc90 = count90 / length(label);
% 
% x1 = [398, 472, 506, 715, 1009, 1654, 1813, 2304, 3104, 2563, 3610, 4924, 5714, 7555, 8088, 9510 ...
%      4801, 2802, 3990, 5484, 655, 1289, 3510, 6409];
% y1 = [394, 437, 491, 752, 1034, 1581, 2154, 2257, 2886, 2589, 3174, 4763, 5110, 5758, 6780, 7469 ...
%      3927, 2777, 3879, 5495, 768, 1326, 3524, 5766];
% x2 = [396, 1639, 2988, 487, 4328, 7953, 9985, 11236, 4983, 2207, 5438, 6833, 2274, 1250, 903, 3849 ...
%      3392, 592, 1155, 2323, 1432, 860, 718, 7967];
% y2 = [419, 1576, 2897, 418, 3632, 8997, 10000, 10000, 4618, 2111, 5835, 6568, 2345, 1113, 839, 3303 ...
%      3524, 544, 1075, 2368, 1212, 935, 790, 6997];
xx = linspace(0,2000,5);
yy = xx;
yyy = 1.1*xx;
yyyy = 0.9*xx;
x1 = label;
y1 = data;
figure;
scatter(x1, y1, 18, 'b', 'filled');
hold on
% scatter(x2, y2, 18, 'r', 'filled');
% hold on
plot(xx, yy, 'k'); 
xlabel('气象站能见度值 (米)');
ylabel('能见度测量值 (米)');
hold on
plot(xx, yyy, 'k');
hold on
plot(xx, yyyy, 'k'); 
