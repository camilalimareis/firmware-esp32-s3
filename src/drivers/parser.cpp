#include "parser.h"

namespace {
    static char lineBuffer[400]; // Verificar se tamanho do buffer é o suficiente
    static uint16_t pos = 0;

    static void processLine(char *line) {

        // Troca as vírgulas por espaço
        char *p = line;
        while (*p) {
            if (*p == ',') 
                *p = ' ';

            p++;
        }

        // ACK
        if (strncmp(line, "ACK:", 4) == 0) {
            ACK::setLast(line + 4);
            return;
        }

        char *saveptr;
        char *tok = strtok_r(line, " ", &saveptr);

        // Leitura do UART do MEGA
        while (tok) {
            char *dp = strchr(tok, ':');
            if (dp) {
                *dp = '\0';
                char *key = tok;
                char *value = dp + 1;

                float val = atof(value);

                // Caso mude aqui, mude no Mega tambem
                if      (strcmp(key, "V") == 0)      Data::data.volts = val;
                else if (strcmp(key, "Pct") == 0)    Data::data.pct = val;
                else if (strcmp(key, "Temp") == 0)   Data::data.temp = (strcmp(value,"NaN")==0) ? NAN:val;
                else if (strcmp(key, "Humi") == 0)   Data::data.humi = (strcmp(value,"NaN")==0) ? NAN:val;
                else if (strcmp(key, "RPM") == 0)    Data::data.rpm = val;
                else if (strcmp(key, "Speed") == 0)  Data::data.speed_kmh = val;
                else if (strcmp(key, "IBAT") == 0)   Data::data.currentBat = val;
                else if (strcmp(key, "IMOT") == 0)   Data::data.currentMot = val;
                else if (strcmp(key, "MIN") == 0)    Data::config.voltageMin = val;
                else if (strcmp(key, "MAX") == 0)    Data::config.voltageMax = val;
                else if (strcmp(key, "WHEEL") == 0)  Data::config.wheel_cm = val;
                else if (strcmp(key, "PPR") == 0)    Data::config.ppr = (uint8_t)atoi(value);
                else if (strcmp(key, "MAXPCT") == 0) Data::config.maxPct = val;
            }

            tok = strtok_r(NULL, " ", &saveptr);
        }
    }

    // Lê byte por byte
    void protocolFeedByte(char c) {
        if (c == '\n') {
            lineBuffer[pos] = '\0';
            processLine(lineBuffer);
            pos = 0;
            return;
        }

        if (c == '\r') return;

        if (pos < sizeof(lineBuffer) - 1) {
            lineBuffer[pos++] = c;
        } else {
            pos = 0; 
        }
    }
}

namespace Parser {
    void loop(char c) {
        protocolFeedByte(c);
    }
}