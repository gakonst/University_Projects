function [results] = data_parser(src,serial, nN, nT,iter)

% input: 
%   src: The file containing the raw results, produced by each method 
%   serial: The file containing the serial results.
%   nN: The number of different number of particles tested
	nT: The number of different number of threads tested
%   iter: Number of repetitions of a single data point. 
%   	  This gets reduced by 1 later because the first iteration is skipped. 
%
% output:
%   results: The structure with the final results (Parallel values divided by the serial values in order to get the speedup)
%       .hash: The time to compute the hash codes
%       .morton: The time to compute the morton encoding
%       .sort: The time of the truncated sort
%       .dataR: The time for data rearrangement
% 		.total: The total time for all functions to execute. 

% Serial Values

system(sprintf('findstr /C:"Time to compute the hash codes" %s > serial_%s_hash.txt', serial,src));
system(sprintf('findstr /C:"Time to compute the morton encoding" %s > serial_%s_morton.txt', serial,src));
system(sprintf('findstr /C:"Time for the truncated radix sort" %s > serial_%s_sort.txt', serial,src));
system(sprintf('findstr /C:"Time to rearrange the particles in memory" %s > serial_%s_data_rear.txt', serial,src));
system(sprintf('findstr /C:"Total time taken" %s > serial_%s_total.txt', serial,src));

% Parallel Values
system(sprintf('findstr /C:"Time to compute the hash codes" %s > hash.txt', src));
system(sprintf('findstr /C:"Time to compute the morton encoding" %s > morton.txt', src));
system(sprintf('findstr /C:"Time for the truncated radix sort" %s > sort.txt', src));
system(sprintf('findstr /C:"Time to rearrange the particles in memory" %s > data_rear.txt', src));
system(sprintf('findstr /C:"Total time taken" %s > total.txt', src));

serial_name = sprintf('serial_%s_hash.txt',src);
fid = fopen(serial_name);
serial_hash_time = fscanf(fid, 'Time to compute the hash codes            : %fs\n');
serial_hash_time = reshape(serial_hash_time,[],nN);
fclose(fid);

serial_name = sprintf('serial_%s_morton.txt',src);
fid = fopen(serial_name);
serial_morton_time = fscanf(fid, 'Time to compute the morton encoding       : %fs\n');
serial_morton_time = reshape(serial_morton_time,[],nN);
fclose(fid);

serial_name = sprintf('serial_%s_sort.txt',src);
fid = fopen(serial_name);
serial_sort_time = fscanf(fid, 'Time for the truncated radix sort         : %fs\n');
serial_sort_time = reshape(serial_sort_time,[],nN);
fclose(fid);

serial_name = sprintf('serial_%s_data_rear.txt',src);
fid = fopen(serial_name);
serial_dataR_time = fscanf(fid, 'Time to rearrange the particles in memory : %fs\n');
serial_dataR_time = reshape(serial_dataR_time,[],nN);
fclose(fid);

serial_name = sprintf('serial_%s_total.txt',src);
fid = fopen(serial_name);
serial_total_time= fscanf(fid, 'Total time taken: %fs\n');
serial_total_time = reshape(serial_total_time,[],nN);
fclose(fid);

% Serial times are 1xnN matrices, each one containing the corresponding
% serial times for each N. To calculate the acceleration we just need to
% divide the corresponding serial values to their parallelized
% bsxfun(@rdivide,x,y) - Divides a matrix by a vector column by column

iter=iter-1;

fid = fopen('hash.txt');
hash_time = fscanf(fid, 'Time to compute the hash codes            : %fs\n');
fclose(fid);
reshape(hash_time,iter,length(hash_time)/iter)
hash_mean = mean(reshape(hash_time, iter, length(hash_time) / iter), 1);
hash_mean = reshape(hash_mean, nT, nN);
hash_accel = bsxfun(@rdivide,serial_hash_time,hash_mean);

fid = fopen('morton.txt');
morton_time = fscanf(fid, 'Time to compute the morton encoding       : %fs\n');
fclose(fid);
morton_mean = mean(reshape(morton_time, iter, length(morton_time) / iter), 1);
morton_mean = reshape(morton_mean, nT, nN);
morton_accel = bsxfun(@rdivide,serial_morton_time,morton_mean);

fid = fopen('sort.txt');
sort_time = fscanf(fid, 'Time for the truncated radix sort         : %fs\n');
fclose(fid);
sort_mean = mean(reshape(sort_time, iter, length(sort_time) / iter), 1);
sort_mean = reshape(sort_mean, nT, nN);
sort_accel = bsxfun(@rdivide,serial_sort_time,sort_mean);

fid = fopen('data_rear.txt');
dataR_time = fscanf(fid, 'Time to rearrange the particles in memory : %fs\n');
fclose(fid);
dataR_mean = mean(reshape(dataR_time, iter, length(dataR_time) / iter), 1);
dataR_mean = reshape(dataR_mean, nT, nN);
dataR_accel = bsxfun(@rdivide, serial_dataR_time, dataR_mean);

fid = fopen('total.txt');
total_time= fscanf(fid, 'Total time taken: %fs\n');
fclose(fid);
total_mean = mean(reshape(total_time, iter, length(total_time) / iter), 1);
total_mean = reshape(total_mean, nT, nN);
total_accel = bsxfun(@rdivide, serial_total_time, total_mean);

results.hash = hash_accel;
results.morton = morton_accel;
results.sort = sort_accel;
results.dataR = dataR_accel;
results.total = total_accel;

threads =0:(nT-1);

figure('Name','Speedup per Function / Thread','NumberTitle','off')
subplot(2,2,1);
plot(threads,results.hash)
xlabel('i | Number of threads =  2^i')
ylabel('Speedup')
title('Hash Codes')
legend('2^j j=20', '2^j j=21', '2^j j=22', '2^j j=23', '2^j j=24', '2^j j=25')

subplot(2,2,2);
%figure('Name','Morton Encoding','NumberTitle','off')
plot(threads,results.morton)
xlabel('i | Number of threads =  2^i')
ylabel('Speedup')

title('Morton Encoding')
legend('2^j j=20', '2^j j=21', '2^j j=22', '2^j j=23', '2^j j=24', '2^j j=25')

subplot(2,2,3);
%figure('Name','Radix Sort','NumberTitle','off')
plot(threads,results.sort)
xlabel('i | Number of threads =  2^i')
ylabel('Speedup')
title('Radix Sort')
legend('2^j j=20', '2^j j=21', '2^j j=22', '2^j j=23', '2^j j=24', '2^j j=25')

subplot(2,2,4);
%figure('Name','Data Rearrangement','NumberTitle','off')
plot(threads,results.dataR)
xlabel('i | Number of threads =  2^i')
ylabel('Speedup')
title('Data Rearrangement')
legend('2^j j=20', '2^j j=21', '2^j j=22', '2^j j=23', '2^j j=24', '2^j j=25')

figure('Name','Total Acceleration','NumberTitle','off')
plot(threads,results.total)
xlabel('i | Number of threads =  2^i')
ylabel('Speedup')
title('Total Acceleration')
legend('2^j j=20', '2^j j=21', '2^j j=22', '2^j j=23', '2^j j=24', '2^j j=25')


end
