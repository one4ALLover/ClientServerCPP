#include <iostream>
#include <string>

int main()

 int bytesReceived;
 bytesReceived = recv(serverSocket);

while (bytesReceived > 0)
{  
    for (int i = 0; i < bytesReceived; i++)
    {
        if(buf[i]!='\n')
        {
            output += buf[i];
        }
        else
        {
            std::cout << output << "\n";
            output.clear();
        }
    }

return 0;
}

