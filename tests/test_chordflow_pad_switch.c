#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "host/midi_fx_api_v1.h"
#include "host/plugin_api_v1.h"

extern midi_fx_api_v1_t* move_midi_fx_init(const host_api_v1_t *host);

static void fail(const char *msg) {
    fprintf(stderr, "FAIL: %s\n", msg);
    exit(1);
}

static void expect_int_param(midi_fx_api_v1_t *api, void *inst, const char *key, int expected) {
    char buf[64];
    int got;
    memset(buf, 0, sizeof(buf));
    if (api->get_param(inst, key, buf, sizeof(buf)) <= 0) {
        fail("get_param returned error");
    }
    got = atoi(buf);
    if (got != expected) {
        fprintf(stderr, "FAIL: %s expected %d got %d\n", key, expected, got);
        exit(1);
    }
}

static void expect_str_param(midi_fx_api_v1_t *api, void *inst, const char *key, const char *expected) {
    char buf[64];
    memset(buf, 0, sizeof(buf));
    if (api->get_param(inst, key, buf, sizeof(buf)) <= 0) {
        fail("get_param returned error");
    }
    if (strcmp(buf, expected) != 0) {
        fprintf(stderr, "FAIL: %s expected %s got %s\n", key, expected, buf);
        exit(1);
    }
}

static void send_note_on(midi_fx_api_v1_t *api, void *inst, int note) {
    uint8_t in[3];
    uint8_t out_msgs[16][3];
    int out_lens[16];

    in[0] = 0x90;
    in[1] = (uint8_t)note;
    in[2] = 100;
    (void)api->process_midi(inst, in, 3, out_msgs, out_lens, 16);
}

int main(void) {
    host_api_v1_t host;
    midi_fx_api_v1_t *api;
    void *inst;

    memset(&host, 0, sizeof(host));
    host.api_version = MOVE_PLUGIN_API_VERSION;

    api = move_midi_fx_init(&host);
    if (!api || !api->create_instance || !api->destroy_instance || !api->set_param || !api->get_param || !api->process_midi) {
        fail("API callbacks missing");
    }

    inst = api->create_instance(".", NULL);
    if (!inst) fail("create_instance failed");

    api->set_param(inst, "pad", "1");
    api->set_param(inst, "root", "c");

    api->set_param(inst, "pad", "32");
    api->set_param(inst, "root", "b");

    send_note_on(api, inst, 67);
    expect_int_param(api, inst, "pad", 32);
    expect_str_param(api, inst, "root", "b");

    send_note_on(api, inst, 36);
    expect_int_param(api, inst, "pad", 1);
    expect_str_param(api, inst, "root", "c");

    send_note_on(api, inst, 99);
    expect_int_param(api, inst, "pad", 32);

    send_note_on(api, inst, 68);
    expect_int_param(api, inst, "pad", 1);

    api->destroy_instance(inst);
    printf("PASS: chordflow pad switching note map\n");
    return 0;
}
