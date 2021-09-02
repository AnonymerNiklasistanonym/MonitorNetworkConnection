import { TestUrlConnectionState, CsvTableElement as TestUrlResult } from "./csvTable";
import dns from "dns";
import tcpp from "tcp-ping";


export const testUrl = async (url: string, attempts: number): Promise<TestUrlResult> => new Promise(resolve => {
    const date = new Date();
    dns.lookup(url, (err, ipAddress) => {
        if (err && err.code === "ENOTFOUND" || ipAddress === undefined) {
            console.error(err);
            return resolve({
                connectionAttempts: -1,
                connectionSpeedMsAvg: -1,
                connectionSpeedMsMax: -1,
                connectionSpeedMsMin: -1,
                connectionState: TestUrlConnectionState.DNS_LOOKUP_FAILED,
                dateIso: date.toISOString(),
                ipAddress,
                url
            });
        } else {

            tcpp.ping({ address: ipAddress, attempts }, (pingErr, data) => {
                const allAttemptsAreErrors = data.results.filter(result => result.err).length === attempts;
                if (pingErr || allAttemptsAreErrors) {
                    console.error(pingErr ? pingErr : data.results.map(result => result.err));
                    return resolve({
                        connectionAttempts: attempts,
                        connectionSpeedMsAvg: -1,
                        connectionSpeedMsMax: -1,
                        connectionSpeedMsMin: -1,
                        connectionState: TestUrlConnectionState.NO_CONNECTION,
                        dateIso: date.toISOString(),
                        ipAddress,
                        url
                    });
                } else {
                    return resolve({
                        connectionAttempts: attempts,
                        connectionSpeedMsAvg: data.avg,
                        connectionSpeedMsMax: data.max,
                        connectionSpeedMsMin: data.min,
                        connectionState: TestUrlConnectionState.CONNECTION,
                        dateIso: date.toISOString(),
                        ipAddress,
                        url
                    });
                }
            });
        }
    });
});

export default {
    testUrl
};
