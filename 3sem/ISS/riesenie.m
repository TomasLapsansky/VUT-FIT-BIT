% VUTBR FIT
% Projekt ISS
% Tomas Lapsansky (xlapsa00)

% OCTAVE
pkg load signal;

% 1. uloha
[y, Fs] = audioread('xlapsa00.wav');

% 2. uloha
len = length(y);
i = fft(y, len);
f = Fs/2*linspace(0,1,len/2+1);
figure(1);
plot(f,abs(i(1:len/2+1)));
xlabel('Frekvencia Hz');
ylabel('Modul spektra');

% 3. uloha
[Xmax,Ymax]=max(abs(i(1:len/2+1)));

% 4. uloha
b = [0.2324 -0.4112 0.2324]
a = [1 0.2289 0.4662];
figure(2)
zplane(b,a);
%stabilny

% 5. uloha
H = freqz(b, a, Fs);
figure(3);
plot((0:len-1)/len*Fs/2, abs(H));
%horni propust

% 6. uloha
filterY = filter(b, a, y);
spec = abs(fft(filterY));
figure(4);
plot(spec(1:Fs/2));
xlabel('Hz');

% 7. uloha
[specMax, specMaxIndex] = max(spec);

% 8. uloha
impulz = [1 1 -1 -1];
impulz = repmat(impulz, 1, 80);
[signal1, signal2] = xcorr(y, impulz);
[max, position] = max(signal1);
v = signal2(position);
time = v/Fs;

% 9. uloha
k = (-50:50);
figure(5);
R = xcorr(y, 50, 'biased');
plot(k, temp);

% 10. uloha
R(61); %indexacia zacina od 1, nie od -50

%
% hist2opt.m
%

function [h,p,r] = hist2(y1,y2,x);
% y1 and y2 are column vectors with data (must have same dimension)
% x  are centers of histogram bins for the 2 coordinates.
%    should be equally spaced !!!
% h is the 2-D histogram (y1 distrib in cols, y2 in rows)
%
% p is the estimate of 2-D prob. dens. function
%
% r is he autocorrelation coefficient computed using the theoretical
% formula (5-8 in Sebesta)
%
% ... histogram computation is not very optimized ...

L = length(x);
N = length(y1);

% alloc for hist
h = zeros(L,L); out = 0; % counter of bad samples...

% make a BIG matrix with all values of x ... will actually do something
% like vector quantization.
% first make col vector out of x, then repeat it N times.
xcol = x(:); bigx = repmat(xcol,1,N);

% make rows out of y1 , y2 and do the 'quantization'
yr = y1(:)'; bigy = repmat(yr,L,1);
[dummy,ind1] = min(abs(bigy - bigx));
yr = y2(:)'; bigy = repmat(yr,L,1);
[dummy,ind2] = min(abs(bigy - bigx));

% now just go through the indices and increment respective cases in h
for ii=1:N,
d1 = ind1(ii);   d2 = ind2(ii);
h(d1,d2) = h(d1,d2) + 1;
end

%%% prob dens: will have to normalize
surf = (x(2) - x(1))^2; % surface of one tile
p = h / N / surf;

%%%% autocor coeff
% make col vector out of x and clone it L times.
x = x(:); X1 = repmat(x,1,L);
% make row vector out of x and clone it L times.
x = x'; X2 = repmat(x,L,1);
% now put it together, don't forget to multipl by tile surface
r = sum(sum (X1 .* X2 .* p)) * surf;

%%% check ...
check = sum(sum (p)) * surf;
disp(['hist2: check -- 2d integral should be 1 and is ' num2str(check)]);
