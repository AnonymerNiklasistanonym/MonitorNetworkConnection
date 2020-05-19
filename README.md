# MonitorNetworkConnection

A tool that can be run via cron job which checks the connection to certain websites.

## Why

This data can tell you if certain websites are often down or if your internet provider has regular outages.

## Run

Build it once:

```sh
npm install
npm run build
npm prune --production
```

Run as often as you want after this build:

```sh
npm run start # or: 'node .'
```

The config data is per default in [`config.json`](config.json):

```json
{
    "attempts": 5,
    "urls": [
        "google.de",
        "youtube.com",
        "en.wikipedia.org"
    ]
}
```

The data is per default created in `data/data.csv`:

| DATE_ISO | URL | IP_ADDRESS | CONNECTION_ATTEMPTS | CONNECTION_STATE | CONNECTION_SPEED_MS_AVG | CONNECTION_SPEED_MS_MAX | CONNECTION_SPEED_MS_MIN |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 2020-05-19T03:13:22.715Z | google.de | 172.217.18.163 | 5 | CONNECTION | 22.6187332 | 26.715923 | 19.612856 |
| 2020-05-19T03:13:22.717Z | youtube.com | 172.217.23.174 | 5 | CONNECTION | 24.8670968 | 37.025768 | 18.336162 |
| 2020-05-19T03:13:22.717Z | en.wikipedia.org | 91.198.174.192 | 5 | CONNECTION | 27.723534800000003 | 29.678412 | 25.775254 |

## Setup Cron Job

Add cron job:

```sh
crontab -e
```

Add entry: (this will create data points every 5 minutes)

```text
*/5 * * * * node PATH/TO/CLONED/REPOSITORY
```
