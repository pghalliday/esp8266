# esphome

Testing the use of esphome to program a `Wemos D1 mini pro` device

Create a `secrets.yaml` file with the following contents

```yaml
substitutions:
  wifi_ssid: YOUR_WIFI_SSID
  wifi_password: YOUR_WIFI_PASSWORD
  fallback_ssid: FALLBACK_AP_SSID
  fallback_password: FALLBACK_AP_PASSWORD
  api_password: API_PASSWORD
  ota_password: OTA_PASSWORD
```

## Requirements

- pipenv - `brew install pipenv`

Then run the following to get a python environment with esphome installed

```
pipenv install
pipenv shell
```

## Build and deploy

From the pipenv shell:

```
esphome laboratory.yaml run
```
