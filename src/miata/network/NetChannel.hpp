/// @file

#pragma once

#include "network.hpp"

class CNetChannel final : public INetChannel
{
public:
	CNetChannel();
	~CNetChannel();

	void Init() override;

	void Setup(netadr_t adr, int qport) override;

	void Transmit(int length, byte *data) override;

	void OutOfBand(netadr_t adr, int length, const byte *data);
	void OutOfBandPrint(netadr_t adr, const char *format, ...);

	bool Process/*Msg*/(CSizeBuffer *apMsg) override;

	bool CanPacket() override;
	bool CanReliable() override;
private:
	static constexpr auto MAX_LATENT{32};

	bool fatal_error{false};

	netsrc_t sock;
	
	int dropped; // between last packet and previous
	
	float last_received; // for timeouts
	int last_sent;       // for retransmits
	
	// the statistics are cleared at each client begin, because
	// the server connecting process gives a bogus picture of the data
	//float frame_latency; // rolling average
	//float frame_rate;

	//int drop_count; // dropped packets, cleared each level
	//int good_count; // cleared each level

	netadr_t remote_address;
	int qport; // qport value to write when transmitting

	// bandwidth estimator
	//double cleartime; // if realtime > nc->cleartime, free to go
	//double rate;      // seconds / byte

	// sequencing variables
	int incoming_sequence;
	int incoming_acknowledged;
	int incoming_reliable_acknowledged; // single bit

	int incoming_reliable_sequence; // single bit, maintained local

	int outgoing_sequence;
	int reliable_sequence;      // single bit
	int last_reliable_sequence; // sequence number of last send

	// reliable staging and holding areas
	CSizeBuffer message; // writing buffer to send to server
	byte message_buf[MAX_MSGLEN];

	// message is copied to this buffer when it is first transfered
	int reliable_length;
	byte reliable_buf[MAX_MSGLEN]; // unacked reliable message

	// time and size data to calculate bandwidth
	int outgoing_size[MAX_LATENT];
	double outgoing_time[MAX_LATENT];
};