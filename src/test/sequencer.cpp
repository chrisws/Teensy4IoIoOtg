////////////////////////////////////////////////////////////////////////////////
// Debug utilities.

static void PrintCue(Cue const *cue, uint16_t oc_enabled) {
  size_t i;
  for (i = 0; i < ARRAY_LEN(cue->oc); ++i) {
    if (oc_enabled & (1 << i)) {
      printf("OC%d { %u %u 0x%x %u } ", i, cue->oc[i].r, cue->oc[i].rs, cue->oc[i].con1, cue->oc[i].inc);
    }
  }
  for (i = 0; i < ARRAY_LEN(cue->port); ++i) {
    printf("P%d { &0x%x |0x%x } ", i, cue->port[i].and, cue->port[i].or);
  }
  printf("\n");
}

static void PrintConfig(ChannelConfig const *cfg) {
  printf("ChannelConfig\n"
         "-------------\n");
#define PRINT(field) printf(#field " = 0x%04x\n", cfg->field)
  PRINT(oc_enabled);
  PRINT(oc_discrete);
  PRINT(oc_idle_change);
  PrintCue(&cfg->idle_cue);
#undef PRINT
}

static void PrintSequence(Sequence const *seq) {
  printf("Sequence\n"
         "--------\n");
  size_t i;
  for (i = 0; i < SequenceSize(seq); ++i) {
    TimedCue const * const timed_cue =
      &seq->buf[(seq->read_count + i) % SEQUENCE_LENGTH];
    printf("Duration = %d\n", timed_cue->time);
    PrintCue(&timed_cue->cue);
  }
}

