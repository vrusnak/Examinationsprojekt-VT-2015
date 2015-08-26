%%
%% Reads two variables from the connected serial port.
%% Plots both variables in real-time.
%%

N=200;
doubleValueFirst = zeros(1, N);
doubleValueSecond = zeros(1, N);
t = zeros(1, N);

flushinput(s);

  for i=1:N
      t(i)= i;
      inValue1 = fscanf(s);
      inValue2 = fscanf(s);
      doubleValueFirst(i) = str2double(inValue1);
	  doubleValueSecond(i) = str2double(inValue2);
      
      plot(t,doubleValueFirst,'r--', t,doubleValueSecond,'b--');
      title('Styrsignal och objektavst�nd')
      xlabel('Sampel')
      ylabel('V�rden')
      legend('Styrv�rde', 'Objektavst�nd')
      drawnow;
  end