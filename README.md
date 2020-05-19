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
