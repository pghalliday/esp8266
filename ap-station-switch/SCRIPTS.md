# Correct password

PRECONDITIONS: {
  Access point turned off
  Wipe flash
  auto-connect on
  auto-reconnect on
}
INITIAL_STATE: {
  MODE = AP
  STATUS = WL_DISCONNECTED
}
ACTION: WiFi.begin with correct password
CHANGE: MODE = STA+AP
CHANGE: STATUS = WL_NO_SSID_AVAIL
ACTION: Turn access point on
CHANGE: STATUS = WL_DISCONNECTED
CHANGE: STATUS = WL_CONNECTED
ACTION: Turn access point off
CHANGE: STATUS = WL_DISCONNECTED
CHANGE: STATUS = WL_NO_SSID_AVAIL
NO_FURTHER_CHANGES

# Incorrect password

PRECONDITIONS: {
  Access point turned off
  Wipe flash
  auto-connect on
  auto-reconnect on
}
INITIAL_STATE: {
  MODE = AP
  STATUS = WL_DISCONNECTED
}
ACTION: WiFi.begin with incorrect password
CHANGE: MODE = STA+AP
CHANGE: STATUS = WL_NO_SSID_AVAIL
ACTION: Turn access point on
REPEAT: 
  CHANGE: STATUS = WL_DISCONNECTED
  CHANGE: STATUS = WL_CONNECT_FAILED
}
ACTION: Turn access point off
CHANGE: STATUS = WL_DISCONNECTED
CHANGE: STATUS = WL_NO_SSID_AVAIL
NO_FURTHER_CHANGES

# WPS timeout

PRECONDITIONS: {
  Access point turned on
  Wipe flash
  auto-connect on
  auto-reconnect on
}
INITIAL_STATE: {
  MODE = AP
  STATUS = WL_DISCONNECTED
}
ACTION: WiFi.beginWPDConfig
CHANGE: MODE = STA+AP
CHANGE: STATUS = WL_IDLE_STATUS
ACTION: Press WPS button on access point
NO_FURTHER_CHANGES

# WPS success

PRECONDITIONS: {
  Access point turned on
  Wipe flash
  auto-connect on
  auto-reconnect on
}
INITIAL_STATE: {
  MODE = AP
  STATUS = WL_DISCONNECTED
}
ACTION: WiFi.beginWPDConfig
ACTION: Press WPS button on access point
CHANGE: MODE = STA+AP
CHANGE: STATUS = WL_CONNECTED
NO_FURTHER_CHANGES

# WPS failure then success

PRECONDITIONS: {
  Access point turned on
  Wipe flash
  auto-connect on
  auto-reconnect on
}
INITIAL_STATE: {
  MODE = AP
  STATUS = WL_DISCONNECTED
}
ACTION: WiFi.beginWPDConfig
CHANGE: MODE = STA+AP
CHANGE: STATUS = WL_IDLE_STATUS
ACTION: WiFi.beginWPDConfig
ACTION: Press WPS button on access point
STATUS = WL_DISCONNECTED
CHANGE: STATUS = WL_CONNECTED
NO_FURTHER_CHANGES

