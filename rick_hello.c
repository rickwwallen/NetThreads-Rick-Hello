//#include <stdio.h>
//#include <arpa/inet.h>
//#include <support.h>
//#include "common.h"
//#include "dev.h"

 #include "support.h"
 #include "pktbuff.h"

 #include "common.h"
 #include "dev.h"

 const char* mystr = "Hello world this is a test pretty awesome. This means that I can send more than just 64 Bytes I LOVE THIS!!!!!!";
 //const char* mystr = "I LOVE THIS!!!!!!";
 //const char* mystr = "Hello world!!!";
 #define STR_SIZE 512

 struct netfpga_to_driver 
 {
  char str[STR_SIZE];
 };
 
 #define PKT_SIZE (sizeof(struct ioq_header) + sizeof(struct netfpga_to_driver))

 int main() 
 {
  struct net_iface iface;
  struct net_iface iface2;
  struct net_iface *ifc = &iface;
  struct net_iface *ifc2 = &iface2;

   //int mytid = nf_tid();
   //
   //if(mytid != 0)
   //  {
   //    nf_stall_a_bit();
   //    nf_lock(LOCK_INIT); // should not get it
   //  }
   //else
   //  {
   //    nf_lock(LOCK_INIT); // should get it on the first attempt
   //    nf_pktout_init();
   //    nf_pktin_init();
   //  }
   //nf_unlock(LOCK_INIT); 

   //only run this program on thread 0
   if (nf_tid() != 0) 
   {
      while (1) {}
   }

   // initialize
   nf_pktout_init();
   nf_pktin_init();

   // iface is not shared, it's on the stack
   arp_init(&iface.arp);
   
   //iface.mac[0] = 0x11;
   //iface.mac[1] = 0x22;
   //iface.mac[2] = 0x33;
   //iface.mac[3] = 0x44;
   //iface.mac[4] = 0x55;
   //iface.mac[5] = 0x66;

   //iface.ip[0] = 192;
   //iface.ip[1] = 168;
   //iface.ip[2] = 0;
   //iface.ip[3] = 200;

   iface.mac[0] = 0x00;
   iface.mac[1] = 0x4e;
   iface.mac[2] = 0x46;
   iface.mac[3] = 0x32;
   iface.mac[4] = 0x43;
   iface.mac[5] = 0x00;

   iface.ip[0] = 192;
   iface.ip[1] = 168;
   iface.ip[2] = 0;
   iface.ip[3] = 100;

   iface2.mac[0] = 0xd4;
   iface2.mac[1] = 0xbe;
   iface2.mac[2] = 0xd9;
   iface2.mac[3] = 0x97;
   iface2.mac[4] = 0xe4;
   iface2.mac[5] = 0x01;

   iface2.ip[0] = 192;
   iface2.ip[1] = 168;
   iface2.ip[2] = 0;
   iface2.ip[3] = 189;

   //iface2.mac[0] = 0x70;
   //iface2.mac[1] = 0x56;
   //iface2.mac[2] = 0x81;
   //iface2.mac[3] = 0xa9;
   //iface2.mac[4] = 0x2c;
   //iface2.mac[5] = 0x39;

   //iface2.mac[0] = 0x01;
   //iface2.mac[1] = 0xe4;
   //iface2.mac[2] = 0x97;
   //iface2.mac[3] = 0xd9;
   //iface2.mac[4] = 0xbe;
   //iface2.mac[5] = 0xd4;

   //iface2.ip[0] = 192;
   //iface2.ip[1] = 168;
   //iface2.ip[2] = 0;
   //iface2.ip[3] = 185;

   // allocate an output buffer
   t_addr *pkt = nf_pktout_alloc(PKT_SIZE);
   //t_addr *pkt = nf_pktout_alloc(PKT_SIZE + sizeof(struct ether_header));
//   struct pkt_buff pkt;
//   pkt_alloc(&pkt,
//		sizeof(struct ioq_header) +
//		sizeof(struct ether_header) +
//		sizeof(struct netfpga_to_driver)
//	);

   // setup the ioq_header
   fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver));
   //fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver)+sizeof(struct ether_header));

   // setup the ethernet header pointer
   //struct ioq_header *rioq = pkt_pull(&pkt, sizeof(struct ioq_header));
   struct ether_header *reth = (struct ether_header*) (pkt + sizeof(struct ioq_header));

   // get a pointer to the payload struct
   //struct netfpga_to_driver* n2d =   (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header));
   struct netfpga_to_driver* n2d =   (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header) + sizeof(struct ether_header));
     //(struct netfpga_to_driver*) (&pkt + sizeof(struct ioq_header) + sizeof(struct ether_header));

   // initialize the message
   //fill_ioq(rioq, 2, sizeof(struct netfpga_to_driver) + sizeof(struct ether_header));
   memcpy(reth->ether_shost, ifc->mac, ETH_ALEN);
   memcpy(reth->ether_dhost, ifc2->mac, ETH_ALEN);
   memset(n2d->str, 0, STR_SIZE); 
   memcpy(n2d->str, mystr, strlen(mystr));
   n2d->str[strlen(mystr)+1] = nf_tid() + 0x30; //thread id in ascii
   //n2d->str[strlen(mystr)+1] = mytid + 0x30; //thread id in ascii

   // send it
   //pkt_push_all(&pkt);
   //nf_pktout_send(pkt.data, &(pkt.len));
   nf_pktout_send(pkt, pkt + PKT_SIZE); 
   

   //int i = 0;
   //int mytid;
   //struct netfpga_to_driver* n2d;
   //t_addr *pkt;
   //while(i <= 1)
   //{
   //   mytid = nf_tid();
   //   
   //   if(mytid != 0)
   //     {
   //       nf_stall_a_bit();
   //       nf_lock(LOCK_INIT); // should not get it
   //     }
   //   else
   //     {
   //       nf_lock(LOCK_INIT); // should get it on the first attempt
   //       nf_pktout_init();
   //       nf_pktin_init();
   //     }
   //   nf_unlock(LOCK_INIT); 

   //   // allocate an output buffer
   //   pkt = nf_pktout_alloc(PKT_SIZE);

   //   // setup the ioq_header
   //   fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver));

   //   // get a pointer to the payload struct
   //   n2d = 
   //     (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header));

   //   // initialize the message
   //   memset(n2d->str, 0, STR_SIZE); 
   //   memcpy(n2d->str, mystr, strlen(mystr));
   //   //n2d->str[strlen(mystr)+1] = nf_tid() + 0x30; //thread id in ascii
   //   n2d->str[strlen(mystr)+1] = mytid + 0x30; //thread id in ascii

   //   // send it
   //   nf_pktout_send(pkt, pkt + PKT_SIZE); 
   //   i = i + 1;
   //}
//#else
//   int mytid = nf_tid();
//   
//   if(mytid != 0)
//     {
//       nf_stall_a_bit();
//       nf_lock(LOCK_INIT); // should not get it
//     }
//   else
//     {
//       nf_lock(LOCK_INIT); // should get it on the first attempt
//       nf_pktout_init();
//       nf_pktin_init();
//     }
//   nf_unlock(LOCK_INIT); 
//   t_addr *pkt = nf_pktout_alloc(PKT_SIZE);
//
//   // setup the ioq_header
//   fill_ioq((struct ioq_header*)pkt, 2, sizeof(struct netfpga_to_driver));
//
//   // get a pointer to the payload struct
//   struct netfpga_to_driver* n2d = 
//     (struct netfpga_to_driver*) (pkt + sizeof(struct ioq_header));
//
//   // initialize the message
//   memset(n2d->str, 0, STR_SIZE); 
//   memcpy(n2d->str, mystr, strlen(mystr));
//   //n2d->str[strlen(mystr)+1] = nf_tid() + 0x30; //thread id in ascii
//   n2d->str[strlen(mystr)+1] = mytid + 0x30; //thread id in ascii
//   nf_pktout_send(pkt, pkt + PKT_SIZE); 
//#endif

   return 0;
 }
