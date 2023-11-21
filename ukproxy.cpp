#include <stdio.h>
#include <pthread.h>

#include <App.h>
#include "ClientApp.h"
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
		/* Overly simple hello world app */
		/*
	uWS::SSLApp({
	  .key_file_name = "misc/key.pem",
	  .cert_file_name = "misc/cert.pem",
	  .passphrase = "1234"
	}).get("/*", [](auto *res, auto *req) {
	    res->end("Hello world!");
	}).listen(3000, [](auto *listen_socket) {
	    if (listen_socket) {
			std::cout << "Listening on port " << 3000 << std::endl;
	    }
	}).run();

	std::cout << "Failed to listen on port 3000" << std::endl;
*/
	uWS::App* app = new uWS::App();
   /*uWS::App().get("/*", [](auto* res, auto* req) {
      res->end("Hello World!");
   }).listen(9001, [](auto *listenSocket) {
      if (listenSocket) {
         fprintf(stdout,"Listening for connections..." );
      }
   }).run();
*/
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