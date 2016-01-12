%%
%% Reads two variables from the connected serial port.
%% Plots both variables in real-time.
%%

N=600;
doubleValue = zeros(1, N);
doubleValueFirst = zeros(1, N);
doubleValueSecond = zeros(1, N);
doubleValueThird = zeros(1, N);
t = zeros(1, N);

  for i=1:N
      
      % Flush old values
      flushinput(s);

      t(i)= i;
      inValue = fscanf(s)
      inValue1 = fscanf(s)
      inValue2 = fscanf(s)
      inValue3 = fscanf(s)
      doubleValue(i) = str2double(inValue);
      doubleValueFirst(i) = str2double(inValue1);
      doubleValueSecond(i) = str2double(inValue2);
      doubleValueThird(i) = str2double(inValue3);
      
      plot(t, doubleValue, 'y--', t,doubleValueFirst,'g--', t,doubleValueSecond,'b--', t,doubleValueThird,'r--');
      title('Styrsignal och objektavstånd')
      xlabel('Sampel')
      ylabel('Avstånd från sensor [cm]')
      legend('Börvärde', 'Styrvärde', 'Objektavstånd', 'Fel')
      grid on
      drawnow;
  end
  
%   End
  fclose(s)
  fopen(s)
  fclose(s)
