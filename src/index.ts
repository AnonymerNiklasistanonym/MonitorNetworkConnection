import config from "./modules/config";
import csvTable from "./modules/csvTable";
import networkChecker from "./modules/networkChecker";
import path from "path";

const csvTableLocation = path.join(__dirname, "..", "data", "data.csv");
const configLocation = path.join(__dirname, "..", "config.json");

(async (): Promise<void> => {
    try {
        // Setup files
        const csvTableAlreadyExists = await csvTable.exists(csvTableLocation);
        if (!csvTableAlreadyExists) {
            await csvTable.setup(csvTableLocation);
        }
        // Get Configuration data
        const configData = await config.read(configLocation);
        // Run tests
        const testResults = await Promise.all(
            configData.urls.map(url => networkChecker.testUrl(url, configData.attempts))
        );
        // Write test results to CSV file
        await csvTable.append(csvTableLocation, testResults, { createFile: !csvTableAlreadyExists });
    } catch (error) {
        throw error;
    }
})().catch(error => {
    console.error(error);
    process.exit(1);
});
