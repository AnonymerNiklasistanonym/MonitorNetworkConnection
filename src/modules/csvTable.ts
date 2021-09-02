import { createObjectCsvWriter } from "csv-writer";
import { promises as fs } from "fs";
import path from "path";

export const enum TestUrlConnectionState {
    CONNECTION = "CONNECTION",
    DNS_LOOKUP_FAILED = "DNS_LOOKUP_FAILED",
    NO_CONNECTION = "NO_CONNECTION",
}

export interface CsvTableElement {
    dateIso: string
    url: string
    ipAddress: string
    connectionAttempts: number
    connectionState: TestUrlConnectionState
    connectionSpeedMsAvg: number
    connectionSpeedMsMax: number
    connectionSpeedMsMin: number
}

const csvTableHeaders = [
    { id: "dateIso", title: "DATE_ISO" },
    { id: "url", title: "URL" },
    { id: "ipAddress", title: "IP_ADDRESS" },
    { id: "connectionAttempts", title: "CONNECTION_ATTEMPTS" },
    { id: "connectionState", title: "CONNECTION_STATE" },
    { id: "connectionSpeedMsAvg", title: "CONNECTION_SPEED_MS_AVG" },
    { id: "connectionSpeedMsMax", title: "CONNECTION_SPEED_MS_MAX" },
    { id: "connectionSpeedMsMin", title: "CONNECTION_SPEED_MS_MIN" }
];

export const exists = async (csvTableLocation: string): Promise<boolean> => {
    try {
        await fs.access(csvTableLocation);
        return true;
    } catch (error) {
        return false;
    }
};

export const setup = async (csvTableLocation: string): Promise<void> => {
    try {
        await fs.access(csvTableLocation);
    } catch (error) {
        // If file cannot be accessed setup directory path and empty csv file
        await fs.mkdir(path.dirname(csvTableLocation), { recursive: true });
    }
};

interface AppendOptions {
    createFile: boolean
}

export const append = async (
    csvTableLocation: string, data: CsvTableElement[], options?: AppendOptions
): Promise<void> => {
    // Setup CSV writer
    const csvWriter = createObjectCsvWriter({
        append: !(options !== undefined && options.createFile),
        header: csvTableHeaders,
        path: csvTableLocation
    });
    // Append data to CSV file
    await csvWriter.writeRecords(data);
};

export default {
    append,
    exists,
    setup
};
