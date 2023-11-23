#include <stdio.h>
#include <pthread.h>

#include <App.h>
#include "wsclient.hpp"

static void usage(const char* argv0)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", argv0);
}

static void* clientThread(void* vargp) 
{ 
	fprintf(stdout, "ClientThread start\n");
	wsclient::WebSocket::pointer ws = wsclient::WebSocket::from_url("ws://192.168.122.1:9999");

	while ((ws->getReadyState() != wsclient::WebSocket::CLOSED)) {
		fprintf(stdout, "ClientThread start poll\n");
		ws->poll();
		ws->dispatchBinary([] (const std::vector<uint8_t>& v) { fprintf(stdout, "ClientThread new msg\n"); });
		sleep(1);
	}

	fprintf(stdout, "ClientThread end\n");
	ws->close();
	pthread_exit(NULL);
}

static void* serverThread(void* vargp)
{
	fprintf(stdout, "ServerThread start\n");
	uWS::App app;
   const int port = 9001;

   app.any("/*", [](auto* res, auto* req) {
      res->end("Hello World!");
   });

   app.listen(port, [&port](auto *listenSocket) {
      if (listenSocket) {
         fprintf(stdout,"Listening for connections..." );
      }
   });

   app.run();

	fprintf(stdout, "ServerThread end\n");
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	if (argc != 3) {
		usage(argv[0]);
		return -1;
	}

   fprintf(stdout, "Proxy start\n");
	pthread_t client_id;
   if (0 != pthread_create(&client_id, NULL, clientThread, NULL)) {
		fprintf(stderr, "client pthread create error\n");
	}

   pthread_t server_id;
   if (0 != pthread_create(&server_id, NULL, serverThread, NULL)) {
		fprintf(stderr, "server pthread create error\n");
	}

	fprintf(stdout, "Proxy wait\n");
	pthread_join(client_id, NULL);
	pthread_join(server_id, NULL);

	fprintf(stdout, "Proxy stop\n");
	return 0;
}