import { promises as fs } from "fs";

export interface ConfigFile {
    attempts: number
    urls: string[]
}

export const read = async (configFileLocation: string): Promise<ConfigFile> => {
    try {
        const data = await fs.readFile(configFileLocation);
        return JSON.parse(data.toString()) as unknown as ConfigFile;
    } catch (error) {
        throw error;
    }
};

export default {
    read
};
