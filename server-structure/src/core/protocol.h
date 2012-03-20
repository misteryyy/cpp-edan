/*
 * Class Protocol: contains definitions of command and error codes
 * for the messaging protocol used in the news system project,
 * C++ Programming course.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace protocol {
  struct Protocol {
    enum {
      /* Command codes, client -> server */
      COM_LIST_NG    = 1,           // list newsgroups
      COM_CREATE_NG  = 2,           // create newsgroup
      COM_DELETE_NG  = 3,           // delete newsgroup
      COM_LIST_ART   = 4,           // list articles
      COM_CREATE_ART = 5,           // create article
      COM_DELETE_ART = 6,           // delete article
      COM_GET_ART    = 7,           // get article
      COM_END        = 8,           // command end
        
      /* Answer codes, server -> client */
      ANS_LIST_NG    = 20,          // answer list newsgroups
      ANS_CREATE_NG  = 21,          // answer create newsgroup
      ANS_DELETE_NG  = 22,          // answer delete newsgroup
      ANS_LIST_ART   = 23,          // answer list articles
      ANS_CREATE_ART = 24,          // answer create article
      ANS_DELETE_ART = 25,          // answer delete article
      ANS_GET_ART    = 26,          // answer get article
      ANS_END        = 27,          // answer end
      ANS_ACK        = 28,          // acknowledge
      ANS_NAK        = 29,          // negative acknowledge
        
      /* Parameters */
      PAR_STRING     = 40,          // string
      PAR_NUM        = 41,          // number
        
      /* Error codes */
      ERR_NG_ALREADY_EXISTS  = 50,  // newsgroup already exists
      ERR_NG_DOES_NOT_EXIST  = 51,  // newsgroup does not exist
      ERR_ART_DOES_NOT_EXIST = 52   // article does not exist
    };
  };
}

#endif
