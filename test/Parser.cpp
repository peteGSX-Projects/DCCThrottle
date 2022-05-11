

int16_t DCCEXParser::splitValues(int16_t result[MAX_COMMAND_PARAMS], const byte *cmd, bool usehex)
{
    byte state = 1;
    byte parameterCount = 0;
    int16_t runningValue = 0;
    const byte *remainingCmd = cmd + 1; // skips the opcode
    bool signNegative = false;

    // clear all parameters in case not enough found
    for (int16_t i = 0; i < MAX_COMMAND_PARAMS; i++)
        result[i] = 0;

    while (parameterCount < MAX_COMMAND_PARAMS)
    {
        byte hot = *remainingCmd;

        switch (state)
        {

        case 1: // skipping spaces before a param
            if (hot == ' ')
                break;
            if (hot == '\0' || hot == '>')
                return parameterCount;
            state = 2;
            continue;

        case 2: // checking sign
            signNegative = false;
            runningValue = 0;
            state = 3;
            if (hot != '-')
                continue;
            signNegative = true;
            break;
        case 3: // building a parameter
            if (hot >= '0' && hot <= '9')
            {
                runningValue = (usehex?16:10) * runningValue + (hot - '0');
                break;
            }
            if (hot >= 'a' && hot <= 'z') hot=hot-'a'+'A'; // uppercase a..z
            if (usehex && hot>='A' && hot<='F') {
                // treat A..F as hex not keyword
                runningValue = 16 * runningValue + (hot - 'A' + 10);
                break;
            }
            if (hot >= 'A' && hot <= 'Z')
            {
                // Since JMRI got modified to send keywords in some rare cases, we need this
                // Super Kluge to turn keywords into a hash value that can be recognised later
                runningValue = ((runningValue << 5) + runningValue) ^ hot;
                break;
            }
            result[parameterCount] = runningValue * (signNegative ? -1 : 1);
            parameterCount++;
            state = 1;
            continue;
        }
        remainingCmd++;
    }
    return parameterCount;
}