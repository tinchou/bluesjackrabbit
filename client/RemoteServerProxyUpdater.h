#ifndef BLUESJACKRABBIT_CLIENT_REMOTESERVERPROXYUPDATER_H
#define BLUESJACKRABBIT_CLIENT_REMOTESERVERPROXYUPDATER_H

#include <sigc++/functors/slot.h>
#include <common/Socket.h>
#include <common/Thread.h>
#include <engine/Vector.h>
#include <vector>
#include <common/GameInitMessage.h>

typedef std::vector<Vector> point_type;
typedef sigc::slot<void, uint32_t, double, double, char, point_type, bool> RendererUpdate;

class RemoteServerProxyUpdater : public Thread {
 public:
  explicit RemoteServerProxyUpdater(RendererUpdate update);
  virtual ~RemoteServerProxyUpdater();
  virtual void run();
  void set_socket(Socket *socket);
  void shutdown();

 private:
  Socket *socket_;
  bool keep_going_;
  RendererUpdate update_functor_;
  void update_objects(GameInitMessage *pMessage);
};

#endif /* BLUESJACKRABBIT_CLIENT_REMOTESERVERPROXYUPDATER_H */
