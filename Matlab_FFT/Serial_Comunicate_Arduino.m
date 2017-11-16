clear
clc
s=serial('COM4','BaudRate',9600);
fopen(s);

x= 0;
figure
hold on
fax_bins = 0 : 128; %N is the number of samples in the signal
xlabel('Frequency (Bins)');
ylabel('Magnitude');
title('Double-sided Magnitude spectrum (bins)');
axis tight
for c = 1:100
clf;
x = fread(s,128);
%readf = fscanf(s);
plot(x);
drawnow;
pause(0.05)
end
fclose(s);
delete(s);
clear s;