typedef char byte;

class serial
{
	public:
		void begin(int);
		char read();
		void write(char);
		int available();
};
