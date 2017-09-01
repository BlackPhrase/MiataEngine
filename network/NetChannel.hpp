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
	
	bool Process() override;
	
	bool CanPacket() override;
	bool CanReliable() override;
private:
	static constexpr auto MAX_LATENT{32};
	
	qboolean	fatal_error;

	float		last_received;		// for timeouts

// the statistics are cleared at each client begin, because
// the server connecting process gives a bogus picture of the data
	float		frame_latency;		// rolling average
	float		frame_rate;

	int			drop_count;			// dropped packets, cleared each level
	int			good_count;			// cleared each level

	netadr_t	remote_address;
	int			qport;

// bandwidth estimator
	double		cleartime;			// if realtime > nc->cleartime, free to go
	double		rate;				// seconds / byte

// sequencing variables
	int			incoming_sequence;
	int			incoming_acknowledged;
	int			incoming_reliable_acknowledged;	// single bit

	int			incoming_reliable_sequence;		// single bit, maintained local

	int			outgoing_sequence;
	int			reliable_sequence;			// single bit
	int			last_reliable_sequence;		// sequence number of last send

// reliable staging and holding areas
	sizebuf_t	message;		// writing buffer to send to server
	byte		message_buf[MAX_MSGLEN];

	int			reliable_length;
	byte		reliable_buf[MAX_MSGLEN];	// unacked reliable message

// time and size data to calculate bandwidth
	int			outgoing_size[MAX_LATENT];
	double		outgoing_time[MAX_LATENT];
};