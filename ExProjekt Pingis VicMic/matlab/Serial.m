%Create serial objekt
s = serial('COM5', 'BAUDRATE', 115200)

%Open serial object
fopen(s)

%%% Write a binary integers to serial port %%%

%Fan initiation
fwrite(s, uint8(1))

%Set parameters
kp = 0.32;
ki = 1.45;
kd = 0.36;
fprintf(s, num2str(kp));
fprintf(s, num2str(ki));
fprintf(s, num2str(kd));


%Set desired value
fwrite(s, uint32(25))

%Turn of fan and clear variables
fwrite(s, uint8(0))

%%%

%Close serial object
fclose(s)
