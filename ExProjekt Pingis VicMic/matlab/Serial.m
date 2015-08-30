%Create serial objekt
s = serial('COM10', 'BAUDRATE', 115200)

%Open serial object
fopen(s)

%%% Write a binary integers to serial port %%%

%Fan initiation
fwrite(s, uint8(1))

%Set desired value to 30cm on the levrer
fwrite(s, uint32(30))

%Turn of fan and clear variables
fwrite(s, uint8(0))

%%%

%Close serial object
fclose(s)
